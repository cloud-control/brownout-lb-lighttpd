Brownout-Aware Load-Balancer
============================

This project contains source code associated to the following article submitted to [SRDS2014](http://www-nishio.ist.osaka-u.ac.jp/conf/srds2014/):

> Cristian Klein, Alessandro Vittorio Papadopoulos, 
> Manfred Dellkrantz, Jonas Dürango,
> Martina Maggio, Karl-Erik Årzén,
> Francisco Hernández-Rodriguez, Erik Elmroth
> "Improving Cloud Service Resilience using Brownout-Aware Load-Balancing"

Brownout is a software engineering paradigm to make cloud services more robust to capacity shortages. It requires the developer to mark a part of the response as optional and only serve it with a probability given by an external parameter, called the _dimmer_. A controller monitors the response time of the service and adjusts the dimmer, so as to keep the application responsive. Details can be found in [this](http://www8.cs.umu.se/~cklein/publications/icse2014-preprint.pdf) article.

The present load-balancer implements two brownout-aware algorithms on top of `lighttpd`: PI-Based Heuristic (PIBH) and Equality Principle-Based Heuristic (EPBH). Specifically, it looks for the `X-Dimmer` header in the HTTP response coming from replicas (or backend servers, in `lighttpd` terminology) representing the last dimmer value computed by the controller, i.e., the probability of a replica to serve a request with optional content. Based on this information, the load-balancer directs requests so as to maximize the amount of optional content served.

Compiling
---------

Our contribution does not add any new dependencies to `lighttpd`, hence, the usual procedure for [compiling from source](http://redmine.lighttpd.net/projects/lighttpd/wiki/InstallFromSource) can be used. On Ubuntu systems, this can be achieved as follows:

	# Install prerequisites
	sudo apt-get install git
	sudo apt-get build-dep lighttpd

	# Clone source code
	git clone https://github.com/cloud-control/brownout-lb-lighttpd.git

	# Initialize autotools
	cd brownout-lb-lighttpd
	./autogen.sh

	# Build
	./configure
	make

Usage
-----

This implementation currently outputs a lot of statistics on standard error. We recommend executing it as follows:

	src/lighttpd -Df lighttpd-brownout-test.conf -m src/.libs/ 2> exp-lb.csv >&2

The provided configuration file `lighttpd-brownout-test.conf` instructs `lighttpd` to use PIBH. For testing EPBH, edit the configuration file as suggested by the comments contained therein.

For questions or comments, please contact Cristian Klein <firstname.lastname@cs.umu.se>.
