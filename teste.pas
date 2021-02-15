program Teste;
	{ lexical level = 1 }

	{ initiate declarative scope }
	var i, j, n: integer;
	    x, y: real;
	    t, w: double;

	procedure A;
		
		{ initiate declarative scope }
		var abacaxi: integer;

		{ imperative scope }
		begin
			abacaxi := j;

			writeln abacaxi

		end;

	function fact(n: integer): integer;

		{ imperative scope }
		begin
			if n > 2 then
				fact := n * fact(n - 1)
			else
				fact := n
		end;

	{ end declarative scope }

	{ imperative scope }

	begin

		t := w * fact (j)
	end.

	O resto é comentário

