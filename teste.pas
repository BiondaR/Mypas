program Teste;
	{ lexical level = 1 }

	{ initiate declarative scope }
	var i, j, n: integer;
	    x, y: real;
	    t, w: double;

	function fact(n: integer): double;
		{ declarative scope} 
		var m: double;
		
		{ imperative scope }
		begin
			if n > 2 then
				m := n * fact(n - 1)
			else
				m := n;
			return m
		end;

	{ end declarative scope }

	{ imperative scope }

	begin

		{t := w * kajdajd (j);}
		t := w * fact (j)
	end.

	O resto é comentário

