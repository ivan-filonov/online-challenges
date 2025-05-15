use std::{
    cell::RefCell,
    io::{BufRead, Write},
    rc::Rc,
};

struct Env<'a> {
    reader: Box<dyn BufRead + 'a>,
    writer: Box<dyn Write + 'a>,
}
impl<'a> Env<'a> {
    fn create(reader: impl BufRead + 'a, writer: impl Write + 'a) -> Self {
        Self {
            reader: Box::new(reader),
            writer: Box::new(writer),
        }
    }
    fn read(&mut self) -> String {
        let mut input = String::new();
        self.reader.read_line(&mut input).unwrap();
        let n: usize = input.trim().parse::<usize>().unwrap();
        input.clear();
        for _ in 0..n {
            self.reader.read_line(&mut input).unwrap();
        }
        eprintln!("input: \"{:?}\"", input);
        input
    }
    fn write(&mut self, s: &str) {
        self.writer.write(s.as_bytes()).unwrap();
    }
}
struct Solver<'a> {
    env: Env<'a>,
    prev: char,
}
/*
ELEMENT = BLOCK | PRIMITIVE_TYPE | KEY_VALUE
BLOCK = "(" ELEMENT (";" ELEMENT)* ")"
PRIMITIVE_TYPE = INT | BOOL | null | STRING
KEY_VALUE = IDENT "=" (BLOCK | PRIMITIVE_TYPE)
*/
#[derive(Debug, Clone, PartialEq, Eq)]
enum Token {
    String(String),
    Ident(String),
    BlockOpen,
    BlockClose,
    Semicolon,
    Assign,
}
#[derive(Debug)]
enum Expr {
    Element(Token),
    KeyValue(Token, Box<Expr>),
    Block(Vec<Box<Expr>>),
}
impl<'a> Solver<'a> {
    fn new(env: Env<'a>) -> Self {
        Self { env, prev: '\n' }
    }

    fn print_pad(&mut self, indent: usize) {
        for _ in 0..indent {
            self.print_char(' ');
        }
    }

    fn print_char(&mut self, c: char) {
        self.prev = c;
        let c = c.to_string();
        self.env.write(c.as_str());
    }

    fn print(&mut self, expr: &Expr, indent: usize) {
        match expr {
            Expr::Element(tok) => {
                if self.prev == '\n' {
                    self.print_pad(indent);
                }
                match tok {
                    Token::Ident(s) => {
                        self.env.write(s);
                    }
                    Token::String(s) => {
                        self.print_char('\'');
                        self.env.write(s);
                        self.print_char('\'');
                    }
                    _ => panic!(),
                }
            }
            Expr::KeyValue(key, value) => {
                if self.prev == '\n' {
                    self.print_pad(indent);
                }
                match key {
                    Token::Ident(s) => {
                        self.env.write(s);
                    }
                    Token::String(s) => {
                        self.print_char('\'');
                        self.env.write(s);
                        self.print_char('\'');
                    }
                    _ => panic!(),
                }
                self.print_char('=');
                if let Expr::Block(_) = value.as_ref() {
                    self.print_char('\n');
                }
                self.print(&value, indent);
            }
            Expr::Block(exprs) => {
                self.print_pad(indent);
                self.print_char('(');

                let mut need_delim = false;
                for e in exprs.iter() {
                    if need_delim {
                        self.print_char(';');
                    }
                    need_delim = true;
                    self.print_char('\n');
                    self.print(e, indent + 4);
                    need_delim = true;
                }

                self.print_char('\n');
                self.print_pad(indent);
                self.print_char(')');
            }
        }
    }

    fn read_block(tokens: &Vec<Token>, start: &mut usize) -> Option<Box<Expr>> {
        if tokens[*start] != Token::BlockOpen {
            return None;
        }
        *start += 1;
        let mut exprs = Vec::new();
        loop {
            let tok = tokens[*start].clone();
            if tok == Token::BlockClose {
                break;
            } else if tok == Token::Semicolon {
                *start += 1;
            } else {
                let elem = Self::read_element(tokens, start);
                exprs.push(elem.unwrap());
            }
        }
        assert_eq!(tokens[*start], Token::BlockClose);
        *start += 1;
        Some(Box::new(Expr::Block(exprs)))
    }
    fn read_key_value(tokens: &Vec<Token>, start: &mut usize) -> Option<Box<Expr>> {
        let key = tokens[*start].clone();
        if *start + 1 >= tokens.len() || tokens[*start + 1] != Token::Assign {
            return None;
        }
        *start += 2;
        if let Some(value) = Self::read_element(tokens, start) {
            if let Expr::KeyValue(_, _) = &*value.as_ref() {
                panic!();
            }
            Some(Box::new(Expr::KeyValue(key, value)))
        } else {
            None
        }
    }
    fn read_element(tokens: &Vec<Token>, start: &mut usize) -> Option<Box<Expr>> {
        let tok = &tokens[*start];
        match tok {
            Token::String(_) | Token::Ident(_) => {
                if let Some(kv) = Self::read_key_value(tokens, start) {
                    Some(kv)
                } else {
                    *start += 1;
                    Some(Box::new(Expr::Element(tok.clone())))
                }
            }
            Token::BlockOpen => Self::read_block(tokens, start),
            _ => None,
        }
    }

    fn solve(&mut self) {
        let tokens = self.get_tokens();
        let mut start = 0;
        while start < tokens.len() {
            let expr = Self::read_element(&tokens, &mut start).unwrap();
            self.print(expr.as_ref(), 0);
            dbg!(expr);
        }
    }
    fn get_tokens(&mut self) -> Vec<Token> {
        let input = self.env.read();
        let mut in_string = false;
        let mut tokens = Vec::new();
        let mut token = String::new();
        for c in input.bytes() {
            if in_string {
                if c == b'\'' {
                    in_string = false;
                    tokens.push(Token::String(token));
                    token = String::new();
                } else {
                    token.push(c as char);
                }
            } else {
                if c == b'\'' {
                    assert!(token.is_empty());
                    in_string = true;
                } else if c.is_ascii_whitespace() {
                    if !token.is_empty() {
                        tokens.push(Token::Ident(token));
                        token = String::new();
                    }
                } else if c == b'(' || c == b')' || c == b'=' || c == b';' {
                    if !token.is_empty() {
                        tokens.push(Token::Ident(token));
                        token = String::new();
                    }
                    tokens.push(match c {
                        b'(' => Token::BlockOpen,
                        b')' => Token::BlockClose,
                        b';' => Token::Semicolon,
                        b'=' => Token::Assign,
                        _ => panic!("c is '{}'", c as char),
                    });
                } else {
                    token.push(c as char);
                }
            }
        }
        if !token.is_empty() {
            tokens.push(Token::Ident(token));
        }
        tokens
    }
}
fn main() {
    let stdin = std::io::stdin().lock();
    let stdout = std::io::stdout().lock();
    let env = Env::create(stdin, stdout);
    let mut solver = Solver::new(env);
    solver.solve();
}
mod test {
    use crate::Solver;

    fn test(input: &str, expected: &str) {
        let mut out = Vec::new();
        let env = super::Env::create(input.as_bytes(), &mut out);
        let mut solver = Solver::new(env);
        solver.solve();
        drop(solver);
        let out = String::from_utf8(out).unwrap();
        let expected = String::from(expected);
        assert_eq!(expected, out);
    }
    #[test]
    fn test1() {
        test(
            r#"4
  

     true
 "#,
            r#"true"#,
        );
    }
    #[test]
    fn test2() {
        test(
            r#"1
'spaces and    tabs'"#,
            r#"'spaces and    tabs'"#,
        );
    }
    #[test]
    fn test3() {
        test(
            r"1
((true))
",
            r"(
    (
        true
    )
)",
        );
    }
}
