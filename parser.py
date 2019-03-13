import csv

R_TIMER_SEC = 65536
duration = 120
                                                                                    
def main():
    arrayOfPacketNums = []
    arrayOfTimeStamps = []

    with open('./data/3.4.3/100_64_nullrdc_25_d_20-2.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            if "Size" in row[0]:
                attributes = row[0].split()
                #packetSize = 64
                #sendingRate = R_TIMER_SEC 
                packetSize = int(attributes[2])
                sendingRate = int((attributes[5])[:-1])
                # print(sendingRate)
            else:
                packetNumAndTimeStamp = row[0].split()
                packetNum = packetNumAndTimeStamp[6]
                timeStamp = packetNumAndTimeStamp[9][:-1]
                arrayOfPacketNums.append(int(packetNum))
                arrayOfTimeStamps.append(float(timeStamp))    
        
        print(getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize))
        print(getLossRatio(arrayOfPacketNums, sendingRate))

def getThroughput(arrayOfPacketNums, arrayOfTimeStamps, packetSize):
    return len(arrayOfPacketNums) * packetSize * 8 / duration

def getLossRatio(arrayOfPacketNums, sendingRate):
    # totalPackets = 3 minutes * 60 secondss * sendingRate
    # print(sendingRate)
    totalPackets = duration * (R_TIMER_SEC / sendingRate)
    # print(totalPackets)
    # print(len(arrayOfPacketNums))
    return (totalPackets - len(arrayOfPacketNums)) / totalPackets

if __name__ == "__main__":
    main()
       