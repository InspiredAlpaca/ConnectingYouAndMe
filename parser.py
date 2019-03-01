import csv

def main():
    arrayOfPacketNums = []
    arrayOfTimeStamps = []

    with open('32_1_26_MAC.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for i in range(0, 20):
            next(csvreader, None)
        attributes = next(csvreader)[3].split()
        packetSize = int(attributes[1])
        sendingRate = int(attributes[4])
        for row in csvreader:
            packetNumAndTimeStamp = row[3].split()
            packetNum = packetNumAndTimeStamp[5]
            timeStamp = packetNumAndTimeStamp[8]
            arrayOfPacketNums.append(int(packetNum))
            arrayOfTimeStamps.append(float(timeStamp))
        
        print(getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize))
        print(getLossRatio(arrayOfPacketNums, sendingRate))

def getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize):
    return len(arrayOfPacketNums) * packetSize * 8 / arrayOfTimeStamps[len(arrayOfTimeStamps)-1]

def getLossRatio(arrayOfPacketNums, sendingRate):
    # totalPackets = 10 minutes * 60 secondss * sendingRate
    totalPackets = 10 * 60 * sendingRate
    return (totalPackets - len(arrayOfPacketNums)) / totalPackets

if __name__ == "__main__":
    main()
       