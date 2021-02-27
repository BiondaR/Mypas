program Teste;
	{ lexical level = 1 }

	{ initiate declarative scope }
	var i, j, n: integer;
	    x, y: real;
	    t, w: double;

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

