#include "filament/filament_linked_list_entry.h"

#include <utility>

FilamentLinkedListEntry::FilamentLinkedListEntry(FilamentLinkedListEntry &&other) noexcept : FilamentLinkedListEntry() {
	moveIntoThis(std::move(other));
}

FilamentLinkedListEntry::~FilamentLinkedListEntry() {
	if(!isEmpty()) {
		remove();
	}
}

FilamentLinkedListEntry &FilamentLinkedListEntry::operator =(FilamentLinkedListEntry &&other) noexcept {
	if(this != &other) {
		remove();

		moveIntoThis(std::move(other));
	}

	return *this;
}

void FilamentLinkedListEntry::moveIntoThis(FilamentLinkedListEntry &&other) noexcept  {
	if(!other.isEmpty()) {
		auto next = other.next();

		other.remove();

		insertBefore(next);
	}
}

void FilamentLinkedListEntry::remove() noexcept {
	auto Next = Flink;
    auto Prev = Blink;

    Next->Blink = Prev;
    Prev->Flink = Next;

    Flink = this;
    Blink = this;
}

void FilamentLinkedListEntry::insertHead(FilamentLinkedListEntry *Entry) noexcept {
    auto Old = Flink;

    Entry->Flink = Old;
    Entry->Blink = this;
    Old->Blink = Entry;
    Flink = Entry;
}

void FilamentLinkedListEntry::insertTail(FilamentLinkedListEntry *Entry) noexcept {
	auto Old = Blink;

    Entry->Flink = this;
    Entry->Blink = Old;
    Old->Flink = Entry;
    Blink = Entry;
}
