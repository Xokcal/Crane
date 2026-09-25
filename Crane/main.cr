import "std/std.cra"

string a = 'manba';
std.printf("hello world\n");
std.printf("%s\n" , a);
std.combine(a , "ok");

class Math{
    string content;
    float PI = 3.14;
    fun:int add(int a , int*b){
        return a + b;
    }
}

// other package import

import "main.cra"

Math math = create Math();
int addR = math.add(14 , 98);
printf("%d" , addR);
