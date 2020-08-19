def getLengthFunc(arr, elements):
    for x in range(0,len(arr)):
        current=arr[x]
        if isinstance(current, list):
            elements+=getLengthFunc(current,0)
        else:
            elements+=1
    return elements


def getLength(arr):
    return getLengthFunc(arr,0)

input=[1,2,[3,1,2,3],4,[5,1]] #Change this variable for others entrys if you desire
print(getLength(input))
