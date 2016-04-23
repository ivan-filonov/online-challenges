uses
    testlib, sysutils;

const
	digits = 6;

var
    ja, pa, eps: extended;
    l: longint;
    i: longint;
    a1, b1, a2, b2, t: longint;

begin
	eps := 1;
	for i := 1 to digits do
		eps := eps / 10;

	l := 0;
	a1 := inf.readinteger;
	while not ans.seekeof do begin
		inc(l);
		a1 := inf.readinteger;
		b1 := inf.readinteger;
		a2 := inf.readinteger;
		b2 := inf.readinteger;
		t  := inf.readinteger;
        ja := ans.readreal;
        pa := ouf.readreal;

        if abs(ja - pa) < eps then
        	continue;

        if (abs(ja) > eps) and (abs(ja - pa) / abs(ja) < eps) then
        	continue;

        quit(_wa, 'Number ' + inttostr(l) + ' - expected: ' + floattostr(ja) + ', found: ' + floattostr(pa) +
        	' at test: ' + inttostr(a1) + ' ' + inttostr(b1) + ' ' + inttostr(a2) + ' ' + inttostr(b2) + ' ' + inttostr(t));
	end;
    quit(_ok, inttostr(l) + ' numbers');
end.