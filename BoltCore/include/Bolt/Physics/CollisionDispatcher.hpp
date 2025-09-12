#pragma once
#include <box2d/box2d.h>
#include "Bolt/Physics/Collision2D.hpp"
#include "Bolt/Physics/PhysicsUtility.hpp"

#include <unordered_map>
#include <functional>
#include <vector>
#include <cstdint>

namespace Bolt {
	// Alias types for clarity
	using ContactBeginCallback = std::function<void(const Collision2D&)>;
	using ContactEndCallback = std::function<void(const Collision2D&)>;
	using ContactHitCallback = std::function<void(const Collision2D&)>;
	
	// Hash functor for b2ShapeId
	struct ShapeIdHash {
		size_t operator()(b2ShapeId id) const noexcept {
			return std::hash<uint64_t>()(b2StoreShapeId(id));
		}
	};

	struct ShapeIdEqual {
		bool operator()(b2ShapeId a, b2ShapeId b) const noexcept {
			return b2StoreShapeId(a) == b2StoreShapeId(b);
		}
	};

	// Dispatcher collects and invokes callbacks
	class CollisionDispatcher {
	public:
		// Register callbacks (all use std::function)
		void registerBegin(b2ShapeId id, ContactBeginCallback cb) {
			m_begin[id].push_back(std::move(cb));
		}
		void registerEnd(b2ShapeId id, ContactEndCallback cb) {
			m_end[id].push_back(std::move(cb));
		}
		void registerHit(b2ShapeId id, ContactHitCallback cb) {
			m_hit[id].push_back(std::move(cb));
		}

		// Called after each world step
		void process(b2WorldId world) {
			b2ContactEvents ev = b2World_GetContactEvents(world);

			// Begin events
			for (int i = 0; i < ev.beginCount; ++i) {
				auto& e = ev.beginEvents[i];
				auto collision2D = Collision2D{ .entityA = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdA),.entityB = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdB) };
				dispatch(e.shapeIdA, collision2D, m_begin);
				dispatch(e.shapeIdB, collision2D, m_begin);
			}
			// End events
			for (int i = 0; i < ev.endCount; ++i) {
				auto& e = ev.endEvents[i];
				auto collision2D = Collision2D{ .entityA = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdA),.entityB = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdB) };
				dispatch(e.shapeIdA, collision2D, m_end);
				dispatch(e.shapeIdB, collision2D, m_end);
			}
			// Hit events
			for (int i = 0; i < ev.hitCount; ++i) {
				auto& e = ev.hitEvents[i];
				auto collision2D = Collision2D{ .entityA = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdA),.entityB = PhysicsUtility::GetNativeEntityFromShapeId(e.shapeIdB) };
				dispatch(e.shapeIdA, collision2D, m_hit);
				dispatch(e.shapeIdB, collision2D, m_hit);
			}
		}

	private:
		template<typename Evt, typename Map>
		void dispatch(b2ShapeId id, const Evt& e, Map& map) {
			auto it = map.find(id);
			if (it == map.end()) return;
			for (auto& cb : it->second) cb(e);
		}

		std::unordered_map<b2ShapeId,
			std::vector<ContactBeginCallback>,
			ShapeIdHash,
			ShapeIdEqual> m_begin;
		std::unordered_map<b2ShapeId, std::vector<ContactEndCallback>, ShapeIdHash, ShapeIdEqual> m_end;
		std::unordered_map<b2ShapeId, std::vector<ContactHitCallback>, ShapeIdHash, ShapeIdEqual> m_hit;
	};
}
