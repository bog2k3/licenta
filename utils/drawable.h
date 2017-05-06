/*
 * drawable.h
 *
 *  Created on: Dec 23, 2014
 *      Author: bogdan
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <memory>

template<typename T> void draw(T* t);

class drawable_wrap {
public:
	template<typename T>
	drawable_wrap(T* t)
		: self_(new model_t<T>(t)) {
	}

	drawable_wrap(const drawable_wrap& w) : self_(w.self_->copy()) {}
	drawable_wrap(drawable_wrap &&w) : self_(std::move(w.self_)) {}
	drawable_wrap& operator = (drawable_wrap const &w) { self_ = decltype(self_)(w.self_->copy()); return *this; }
	drawable_wrap& operator = (drawable_wrap &&w) { self_ = std::move(w.self_); return *this; }

	bool equal_value(drawable_wrap const& w) const {
		return self_->getRawPtr() == w.self_->getRawPtr();
	}

	void draw() {
		self_->draw_();
	}

private:
	struct concept_t {
		virtual ~concept_t() noexcept = default;
		virtual void draw_() = 0;
		virtual concept_t* copy()=0;
		virtual void* getRawPtr() = 0;
	};
	template<typename T>
	struct model_t : concept_t {
		T* obj_;
		model_t(T* x) : obj_(x) {}
		~model_t() noexcept {};
		void draw_() override {
			drawImpl(obj_, true);
		}
		concept_t* copy() override {
			return new model_t<T>(obj_);
		}
		void* getRawPtr() {
			return obj_;
		}

		template<typename T1>
		static decltype(&T1::draw) drawImpl(T1* t, bool dummyToUseMember) {
			t->draw();
			return nullptr;
		}
		template<typename T1>
		static void drawImpl(T1* t, ...) {
			::draw(t);
		}
	};

	std::unique_ptr<concept_t> self_;
};

#endif /* DRAWABLE_H_ */
