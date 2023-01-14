#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include <vector>
#include "ComponentTraits.h"

namespace EAMFUP::Component {

// Using a template template argument for show
template <typename SampleType, template <typename> typename Container>
struct PassThrough
        : public Traits::ComponentTraits<Traits::EffectComponentTag> {
    using InputType = SampleType;
    using OutputType = SampleType;

    PassThrough() noexcept = default;

    void run(Container<SampleType>&) {}
};

} // namespace EAMFUP::Component

#endif /* ifndef PASSTHROUGH_H */
