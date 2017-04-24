/*
 * UpdateList.cpp
 *
 *  Created on: Jun 18, 2016
 *      Author: bog
 */

#include "UpdateList.h"
#include "parallel.h"
#include <algorithm>
#include <array>

void UpdateList::update(float dt, ThreadPool *pool /*= nullptr*/) {
	// add pending:
	std::move(pendingAdd_.begin(), pendingAdd_.end(), std::back_inserter(list_));
	pendingAdd_.clear();
	// remove pending:
	list_.erase(std::remove_if(list_.begin(), list_.end(), [this] (const updatable_wrap& x) {
		for (void* ptr : pendingRemove_)
			if (x.equal_raw(ptr))
				return true;
		return false;
	}), list_.end());
	pendingRemove_.clear();

	// do update on current elements:
	if (pool) {
		// run in thread pool
		parallel_for(list_.begin(), list_.end(), *pool,
			[dt](auto &x) {
				x.update(dt);
			});
	} else {
		// run them on this thread
		for (auto &e : list_)
			e.update(dt);
	}
}

