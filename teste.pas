program Teste;
	{ lexical level = 1 }

	{ initiate declarative scope }
	var i, j, n, w: integer;
	    x, y: real;
	    t: double;

	procedure troca(var valor1, valor2: integer);
  		var aux: integer;

  		begin
    		aux := valor1;
    		valor1 := valor2;
    		valor2 := aux;
  		end;

	function fact(n: integer): double;
		{ declarative scope} 
		var m: double;
		
		{ imperative scope }
		begin
			if n > 2 then
				m := i * fact(n - 1)
			else
				m := n;
			return m
		end;

	{ end declarative scope }

	{ imperative scope }

	begin

		{t := w * kajdajd (j);}
		t := w * fact (j);
		troca(i, j)
	end.

	O resto é comentário

