def add(a,b) do
    a + b
end

def funky_multiply(a,b) do
    if a < 2 do
        a + b
    else
        a * b
        a * b + 2
    end
end
def main() do
    print(funky_multiply(3,4) * 25)
end
