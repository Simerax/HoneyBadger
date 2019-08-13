
def loop(x) do
    if x < 1 do
        0
    else
        print(x)
        loop(x - 1)
    end
end

def main() do
    loop(10)
end
