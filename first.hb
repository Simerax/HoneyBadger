// this file contains a few examples of the language


// fibonacci numbers
def fib(x) do
    if x < 2 do
        x
    else
        fib(x - 1) + fib(x - 2)
    end
end

// we dont have loops its recursive!
def loop(x) do
    if x < 1 do
        0
    else
        print(x)
        loop(x - 1)
    end
end


// every programm needs a main, as you expected this is the entrypoint for the programm
def main() do
    print(fib(22))
    loop(10)
end
