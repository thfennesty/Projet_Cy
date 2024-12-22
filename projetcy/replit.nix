{ pkgs }: {
	deps = [
   pkgs.valgrind-light
   pkgs.bc
		pkgs.clang
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}