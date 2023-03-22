build_req:
	mkdir -p obj

build_p1: build_req p1/main.cpp
	g++ -pthread -lrt p1/main.cpp -o obj/p1

build_p3: build_req p3/main.cpp
	g++ -pthread -lrt -DT1 p3/main.cpp -o obj/p3_t1
	g++ -pthread -lrt p3/main.cpp -o obj/p3_t2

build_plotter: build_req plotter/src/main.rs
	cargo build --manifest-path plotter/Cargo.toml --release
	mv plotter/target/release/plotter obj/

p1: build_p1
	obj/p1 >> obj/p1_result
	echo "P1 finish!!"

p2:

p3: build_p3
	chrt -f 99 obj/p3_t1 obj/p3_t1_result_a &
	chrt -f 98 obj/p3_t2 obj/p3_t2_result_a &

	sleep 1800

	chrt -f 98 obj/p3_t1 obj/p3_t1_result_b &
	chrt -f 99 obj/p3_t2 obj/p3_t2_result_b &

	sleep 1800

	echo "P3 finish"

run: p1 p2 p3

clean:
	rm -f obj/*

