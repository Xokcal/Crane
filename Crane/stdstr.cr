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

for string s in strArrays{
    printf("%s\n" , s);
}

for int i in (0 , 10){
    std.printf("%d\n" , i);
}

for int i in (array.len){
    std.printf("%d\n" , i);
}

float rp = (pi * 2) \ 4 * std.math.random();
float maxp = std.math.max(pi , rp);
std.printf("max is : %s" , maxp);