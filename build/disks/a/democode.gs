using "./stdlib.gs";

func main(args[])
{
    stdio.println("Hello world!");

    var num = 10;
    if (num > 10) { stdio.println("Number is big"); }
    else { stdio.println("Number is small"); }

    num += foo(69) + bar(420);
}

func foo(x) { return x * 420; }

func bar(x) { return x * 69; }