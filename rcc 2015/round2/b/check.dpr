uses
    testlib, sysutils;

const
	digits = 6;

var
    ja, pa, eps: extended;
    l: longint;
    i: longint;

begin
	eps := 1;
	for i := 1 to digits do
		eps := eps / 10;

	l := 0;
	while not ans.seekeof do begin
		inc(l);
        ja := ans.readreal;
        pa := ouf.readreal;

        if abs(ja - pa) < eps then
        	continue;

        if (abs(ja) > eps) and (abs(ja - pa) / abs(ja) < eps) then
        	continue;

        quit(_wa, 'Number ' + inttostr(l) + ' - expected: ' + floattostr(ja) + ', found: ' + floattostr(pa));
	end;
    quit(_ok, inttostr(l) + ' numbers');
end.