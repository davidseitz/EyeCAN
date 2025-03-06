import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import {Heading} from "@chakra-ui/react";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";
import FilterPreviewCard from "@/components/dashboard/sidebar/filter-collection/FilterPreviewCard.tsx";
import mockData from "@/data/MockData.ts";

function FilterCollection() {

    return (
        <DropShadowContainer padding={{paddingX: 8, paddingY: 6}} height={"55%"}>
            <Heading marginBottom={4} fontSize="2xl" fontWeight={700}>
                Filter Collection
            </Heading>
            <ScrollableContainer height="90%" margins={{marginX: 0, marginY: 0}} fade={{show: true, size: 100}}>
                {mockData.filters.map((filter) =>
                    <FilterPreviewCard
                        key={filter.id}
                        filter={filter}
                    />)
                }
            </ScrollableContainer>
        </DropShadowContainer>
    )
}

export default FilterCollection;