
haversine_gen: 
	g++ haversine_gen.cpp -o haversine_gen.out

haversine_main: 
	g++ haversine_main.cpp -o haversine_main.out

timed_haversine_main: 
	g++ timed_haversine_main.cpp -o timed_haversine_main.out

profiled_haversine_main: 
	g++ profiled_haversine_main.cpp -o profiled_haversine_main.out

double_counted_haversine_main: 
	g++ double_counted_haversine_main.cpp -o double_counted_haversine_main.out

nesting_haversine_main: 
	g++ nesting_haversine_main.cpp -o nesting_haversine_main.out

final_haversine_main_no_profile:
	g++ final_haversine_main.cpp -o final_haversine_main.out

final_haversine_main_profiled:
	g++ -DPROFILER=1 final_haversine_main.cpp -o final_haversine_main.out

os_timer_main:
	g++ os_timer_main.cpp -o os_timer_main.out

cpu_timer_main:
	g++ cpu_timer_main.cpp -o cpu_timer_main.out

cpu_timer_guessfreq_main:
	g++ cpu_timer_guessfreq_main.cpp -o cpu_timer_guessfreq_main.out

clean:
	rm -f *.out


.PHONY: haversine_gen haversine_main timed_haversine_main profiled_haversine_main os_timer_main cpu_timer_main cpu_timer_guessfreq_main double_counted_haversine_main nesting_haversine_main final_haversine_main_profiled final_haversine_main_no_profile
