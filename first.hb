// this file contains a few examples of the language


// fibonacci numbers
def fib(x:F64) : F64 do
    if x < 2 do
        x
    else
        fib(x - 1) + fib(x - 2)
    end
end

// we dont have loops its recursive!
def loop(x:F64) : F64 do
    if x < 1 do
        0
    else
        print(x)
        loop(x - 1)
    end
end


// every programm needs a main, as you expected this is the entrypoint for the programm
def main() : F64 do
    //#print(fib(22))
    loop(1000000)
    let x = 25
    print(x)
    let y = x + 25
    print(y)
end
