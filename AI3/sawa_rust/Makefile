a.out: src/main.rs
	cargo build --release
	cp ./target/release/ai a.out
a.out_debug: src/main.rs
	cargo build
	cp ./target/debug/ai a.out
dev: src/main.rs
	cargo build
main.cpp:
	cp ../../common/hasi_template2/main.cpp ./
common.hpp:
	cp ../../common/hasi_template2/common.hpp ./
