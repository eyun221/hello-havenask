#ifndef FUTURE_LITE_UNIT_H
#define FUTURE_LITE_UNIT_H


namespace future_lite {

struct Unit {
    constexpr bool operator==(const Unit&) const
    {
        return true;
    }
    constexpr bool operator!=(const Unit&) const
    {
        return false;
    }
};


}

#endif //FUTURE_LITE_UNIT_H
