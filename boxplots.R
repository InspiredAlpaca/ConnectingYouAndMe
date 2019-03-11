plotthroughput <- function(data, name) {
	boxplot(data, range=0, ylab="Throughput (bps)", main=name)
}

plotloss <- function(data, name) {
	boxplot(data, range=0, ylab="Data Loss", main=name)
}