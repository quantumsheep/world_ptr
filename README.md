# World Smart Pointer
This library provide a single header file containing `world_ptr`, a smart pointer that uses a pointer map to store a counter which, arrived at `0`, delete the referenced pointer. Its functioning is very near `std::shared_ptr` but is surely a lot less safer *- but at least you won't have memory leaks*.

The motivation behind this was to return a raw pointer as a smart pointer without having it as a weak smart pointer or raw pointer which doesn't ensure its lifetime after behind stored somewhere else.
