import csv

R_TIMER_SEC = 65536

def main():
    arrayOfPacketNums = []
    arrayOfTimeStamps = []

    with open('100_32_mac_26_d_2-1.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            if "Size" in row[0]:
                attributes = row[0].split()
                packetSize = int(attributes[2])
                sendingRate = int((attributes[5])[:-1])
            else:
                packetNumAndTimeStamp = row[0].split()
                packetNum = packetNumAndTimeStamp[6]
                timeStamp = packetNumAndTimeStamp[9][:-1]
                arrayOfPacketNums.append(int(packetNum))
                arrayOfTimeStamps.append(float(timeStamp))
        
        print(getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize))
        print(getLossRatio(arrayOfPacketNums, sendingRate))

def getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize):
    return len(arrayOfPacketNums) * packetSize * 8 / (arrayOfTimeStamps[len(arrayOfTimeStamps)-1] - arrayOfTimeStamps[0])

def getLossRatio(arrayOfPacketNums, sendingRate):
    # totalPackets = 3 minutes * 60 secondss * sendingRate
    totalPackets = 2 * 60 * 1 / (sendingRate / R_TIMER_SEC)
    return (totalPackets - len(arrayOfPacketNums)) / totalPackets

if __name__ == "__main__":
    main()
       