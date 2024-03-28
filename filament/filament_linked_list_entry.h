#ifndef FILAMENT_FILAMENT_LINKED_LIST_ENTRY_H
#define FILAMENT_FILAMENT_LINKED_LIST_ENTRY_H

class FilamentLinkedListEntry {
public:
	inline FilamentLinkedListEntry() noexcept : Flink(this), Blink(this) {

	}

	~FilamentLinkedListEntry();

	FilamentLinkedListEntry(const FilamentLinkedListEntry &other) = delete;
	FilamentLinkedListEntry &operator =(const FilamentLinkedListEntry &other) = delete;

	FilamentLinkedListEntry(FilamentLinkedListEntry &&other) noexcept;
	FilamentLinkedListEntry &operator =(FilamentLinkedListEntry &&other) noexcept;

	inline bool isEmpty() const noexcept  {
		return Flink == this;
	}

	void remove() noexcept;

	void insertHead(FilamentLinkedListEntry *entry) noexcept;
	void insertTail(FilamentLinkedListEntry *entry) noexcept;

	inline void insertBefore(FilamentLinkedListEntry *entry) noexcept {
		insertTail(entry);
	}

	inline void insertAfter(FilamentLinkedListEntry *entry) noexcept {
		insertHead(entry);
	}

	inline FilamentLinkedListEntry *next() const noexcept {
		return Flink;
	}

	inline FilamentLinkedListEntry *prev() const noexcept {
		return Blink;
	}

private:
	void moveIntoThis(FilamentLinkedListEntry &&other) noexcept;

    FilamentLinkedListEntry *Flink;
    FilamentLinkedListEntry *Blink;

};

#endif
