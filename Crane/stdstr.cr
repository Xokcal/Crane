import "stdcra/std.cr"

std.printf("hello world!"\n);
std.printf("hello Crane!"\n);
std.printf("hello Xokcal!"\n);
std.printf("Do you love Crane?"\n);

int pi = 1363;
std.printf("%d" , pi);
string[] strArr = {"hello" , "Crane"  ,"Xokcal" , "Sranmes"};

for string s in strArr index:int i {
    std.printf("string Array : %s\n" , s);
}

float rp = (pi * 2) \ 4 * std.math.random();
float maxp = std.math.max(pi , rp);
std.printf("max is : %s" , maxp);