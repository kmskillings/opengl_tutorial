import random
import math

random.seed()

def getUnitBall():
    while True:
        x = random.random() * 2 - 1
        y = random.random() * 2 - 1
        z = random.random() * 2 - 1
        if x * x + y * y + z * z <= 1.0:
            return (x, y, z)
        
def getUnitSphere():
    (x, y, z) = getUnitBall()
    mag = math.sqrt(x * x + y * y + z * z)
    return (x / mag, y / mag, z / mag)

def getRotation():
    axis = getUnitSphere()
    angle = random.random() * math.pi * 2
    return (angle, axis)

def getTransform():
    position = getUnitBall()
    orientation = getRotation()
    rotation = getRotation()
    return (position, orientation, rotation)

def stringifyTransform(transform):
    # Transform is a tuple of the form:
    #  position (x, y, z)
    #  orientation (angle, axis(x, y, z))
    #  rotation (angle, axis(x, y, z))
    return f"{transform[0][0]}, {transform[0][1]}, {transform[0][2]}, {transform[1][0]}, {transform[1][1][0]}, {transform[1][1][1]}, {transform[1][1][2]}, {transform[2][0]}, {transform[2][1][0]}, {transform[2][1][1]}, {transform[2][1][2]}"

def generateTransforms(count, outputPath):
    outString = ""
    for i in range(count):
        transform = getTransform()
        outString = outString + stringifyTransform(transform)
        outString = outString + ",\n"
    with open(outputPath, "w") as outputFile:
        outputFile.write(outString)