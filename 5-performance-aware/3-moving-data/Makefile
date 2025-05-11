
bandwidth_haversine_main_no_profile:
	g++ bandwidth_haversine_main.cpp -o bandwidth_haversine_main.out

bandwidth_haversine_main_profiled:
	g++ -DPROFILER=1 bandwidth_haversine_main.cpp -o bandwidth_haversine_main.out

clean:
	rm -f *.out


.PHONY: bandwidth_haversine_main_profiled bandwidth_haversine_main_no_profile
