program sort (input,output);
	var a : array[0..10] of integer;
	x : integer;

	procedure readarray;
	var i : integer;
	begin
		for i:=1 to 9 do read(a[i])
	end;

	procedure exchange (i,j:integer)
	begin
		x:=a[i]; a[i]:=a[j]; a[j]:=x
	end;

	procedure quicksort (m,n:integer);
	var k,v : integer;
	function partition (y,z :integer):integer;
		var i,j : integer;
		begin …a…； …v…；
			exchange(i,j); ……
		end;
	begin ……
		k=partition(m,n);
		quicksort(m,k-1);
		quicksort(k+1,n); ……
	end;


	begin readarray; quicksort(1,9)
	end. 