#pragma once
#include <LogicPin.h>
#include <vector>

#define INIT_PINS(count) \
        for (int i = 0; i < count; i++) \
            pins_.push_back(std::make_shared<LogicPin>(weak_from_this()));


class LogicComponent : public std::enable_shared_from_this<LogicComponent> {
protected:
    std::vector<std::shared_ptr<LogicPin>> pins_;

public:
    template<typename Derived, typename... Args>
    static std::shared_ptr<LogicComponent> create(Args&&... args) {
        static_assert(std::is_base_of_v<LogicComponent, Derived>);
        std::shared_ptr<LogicComponent> self = std::make_shared<Derived>(std::forward<Args>(args)...);
        self->initPins();
        return self;
    }

    virtual void handle() = 0;
    virtual void initPins() = 0; // add pins to pins_ at derived class

    virtual std::vector<std::weak_ptr<LogicPin>> pins() {
        return std::vector<std::weak_ptr<LogicPin>>{pins_.begin(), pins_.end()};
    }

    virtual ~LogicComponent() {

    }
};