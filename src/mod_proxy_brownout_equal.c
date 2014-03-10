#include "mod_proxy_brownout_equal.h"

#define data_proxy data_fastcgi

void mod_proxy_brownout_equal_update_queue_offsets(data_array *extension, float dt) {
	/* Here we try to implement the following Python code:
		dt = self.sim.now - self.lastDecision
		if dt > 1: dt = 1
		for i in range(0,len(self.backends)):
			# Gain
			gamma = .1 * dt
			gammaTr = .01 * dt
			
			# Calculate the negative deviation from the average
			e = self.lastThetas[i] - avg(self.lastThetas)
			# Integrate the negative deviation from the average
			self.queueOffsets[i] += gamma * e
			# Anti-windup
			self.queueOffsets[i] -= gammaTr * (self.queueOffsets[i] - self.queueLengths[i])
			self.lastThetaErrors[i] = e
		self.lastDecision = self.sim.now
		
		# choose replica with shortest (queue + queueOffset)
		request.chosenBackendIndex = \
			min(range(0, len(self.queueLengths)), \
			key = lambda i: self.queueLengths[i]-self.queueOffsets[i])
	*/
	int numReplicas = (int) extension->value->used;
	int i;

	float gamma = .1 * dt;
	float gammaTr = .01 * dt;

	float avgTheta = 0;
	/* Compute average of thetas */
	for (i = 0; i < numReplicas; i++) {
		data_proxy *host = (data_proxy *)extension->value->data[i];
		float lastTheta = host->lastTheta;

		avgTheta += lastTheta;
	}
	avgTheta /= numReplicas;

	/* Do control stuff */
	for (i = 0; i < numReplicas; i++) {
		data_proxy *host = (data_proxy *)extension->value->data[i];
		float lastTheta = host->lastTheta;
		float lastLastTheta = host->lastLastTheta;
		int queueLength = host->usage;
		float *pQueueOffset = &host->queueOffset;

		// Calculate the negative deviation from the average
		float e = lastTheta - avgTheta;
		// Integrate the negative deviation from the average
		*pQueueOffset += gamma * e;
		// Anti-windup
		*pQueueOffset -= gammaTr * (*pQueueOffset - queueLength);
	}
}
