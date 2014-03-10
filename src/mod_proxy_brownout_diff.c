#include "mod_proxy_brownout_diff.h"

#define data_proxy data_fastcgi

void mod_proxy_brownout_diff_update_queue_offsets(data_array *extension, float dt) {
	/* Here we try to implement the following Python code:
		dt = self.sim.now - self.lastDecision
		if dt > 1: dt = 1

		for i in range(0,len(self.backends)):
			# Gain
			Kp = 0.25
			Ti = 5.0
			gammaTr = .01

			# PI control law
			e = self.lastThetas[i] - self.lastLastThetas[i]
			self.queueOffsets[i] += (Kp * e + (Kp/Ti) * self.lastThetas[i]) * dt

			# Anti-windup
			self.queueOffsets[i] -= gammaTr * (self.queueOffsets[i] - self.queueLengths[i]) * dt
			self.lastThetaErrors[i] = e

		self.lastDecision = self.sim.now
		
		# choose replica with shortest (queue + queueOffset)
		request.chosenBackendIndex = \
			min(range(0, len(self.queueLengths)), \
			key = lambda i: self.queueLengths[i]-self.queueOffsets[i])
	*/
	int numReplicas = (int) extension->value->used;
	int i;

	float Kp = 0.5;
	float Ti = 5.0;
	float gammaTr = 0.01;

	/* Do control stuff */
	for (i = 0; i < numReplicas; i++) {
		data_proxy *host = (data_proxy *)extension->value->data[i];
		float lastTheta = host->lastTheta;
		float lastLastTheta = host->lastLastTheta;
		int queueLength = host->usage;
		float *pQueueOffset = &host->queueOffset;

		// PI control law
		float e = lastTheta - lastLastTheta;
		*pQueueOffset += (Kp * e + (Kp/Ti) * lastTheta) * dt;

		// Anti-windup
		*pQueueOffset -= gammaTr * (*pQueueOffset - queueLength) * dt;
	}
}
