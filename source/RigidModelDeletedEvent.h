#include "GameEvent.h"

class RigidModelEntity;

class RigidModelDeletedEvent : public GameEvent {
public:
	class RigidModelDeletedEvent(const std::string &name, void *ptr);

	void* getPointer() const;

private:
	void *mPtrToDelete;
};