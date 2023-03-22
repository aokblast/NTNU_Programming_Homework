build_req: obj
	mkdir -p obj

build_p1: build_req p1/main.cpp
	g++ -pthread -lrt p1/main.cpp -o obj/p1

build_p3: build_req p3/main.cpp
	g++ -pthread -lrt -DT1 p3/main.cpp -o obj/p3_t1
	g++ -pthread -lrt p3/main.cpp -o obj/p3_t2

build_plotter: build_req plotter/src/main.rs
	cargo build --manifest-path plotter/ --release
	mv plotter/target/release/plotter obj/

p1: build_p1
	obj/p1 >> obj/p1_result

p2:

p3: build_p3
	chrt -f 99 obj/p3_t1 obj/p3_t1_result &
	chrt -f 98 obj/p3_t2 obj/p3_t2_result &

	timeout 1800

	pkill -9 -f p3_t1
	pkill -9 -f p3_t2

	chrt -f 98 obj/p3_t1 obj/p3_t1_result &
	chrt -f 99 obj/p3_t2 obj/p3_t2_result &

	timeout 1800

	pkill -9 -f p3_t1
	pkill -9 -f p3_t2


run: p1 p2 p3

clean:
	rm -f obj/*

