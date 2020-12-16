import os
import random


# 根据字母表zigma中的字母随机生成长度为len的字符串
def generator(length, zigma):
    str = ""
    for i in range(length):
        j = random.randint(0, len(zigma) - 1)
        str = str + zigma[j]
    # print("input: " + str)
    return str

# 测试回文串
def test1(input):
    if input == input[::-1] :
        return True
    return False
    
# 测试aibjaibj
def test2(input):
    i = 0
    j = 0
    while i < len(input) and input[i] == "a" :
        i += 1
    while i + j < len(input) and input[i + j] == "b":
        j += 1
    if len(input) != 2 * (i + j) :
        return False
    if input[i+j:] == input[0:i+j]:
        return True
    return False

# 测试一进制乘法
def test3(input):
    i = 0
    j = 0
    k = 0
    while i < len(input) and input[i] == "1":
        i += 1
    if i == 0 or i == len(input):
        return False
    if input[i] != "x":
        return False
    while i+1+j < len(input) and input[i+1+j] == "1":
        j += 1
    if j == 0 or i+1+j == len(input):
        return False
    if input[i+1+j] != "=":
        return False
    while i+1+j+1+k < len(input) and input[i+1+j+1+k] == "1":
        k += 1
    if k == 0:
        return False
    if i+1+j+1+k != len(input):
        return False
    if i * j != k:
        return False
    return True

def main():
    #规定字母表
    zigma = "1x="
    
    cnt = 0
    
    for i in range(100):

        input = generator(random.randint(901, 1000), zigma) # 调节生成随机串的长度

        # tm_file = "palindrome_detector_2tapes.tm "
        # tm_file = "aibjaibj.tm "
        tm_file = "unary_multi.tm "

        os.system("turing " + tm_file + input + " > tmp")
        file = open("tmp", "r")
        myres = file.readline().strip("\n\r")
        pyres = ""

        if test3(input):
            pyres = "true"
        else:
            pyres = "false"
        if myres != pyres:
            print("bad test case : " + input)
            print("my result is " + str(myres))
            print("python's result is " + str(pyres))
        else:
            # print("good")
            cnt = cnt + 1

    print("total good test cases : " + str(cnt))


if __name__ == "__main__":
    main()