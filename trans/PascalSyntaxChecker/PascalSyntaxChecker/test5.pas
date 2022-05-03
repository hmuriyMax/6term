program tester@;

type h = record
  a:int;
end;
var b:int;


begin
  if true then
    write(1)
  else
    write(2);

  repeat 
    read(b)
  until b<5
end.