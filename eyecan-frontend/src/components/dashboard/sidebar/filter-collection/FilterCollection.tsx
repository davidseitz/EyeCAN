import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import {Heading} from "@chakra-ui/react";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";
import FilterPreviewCard from "@/components/dashboard/sidebar/filter-collection/FilterPreviewCard.tsx";
import mockData from "@/data/MockData.ts";
import {Dataset} from "@/types/dataset.ts";
import {useState} from "react";

interface FilterCollectionProps {
    selectedDatasets: Dataset[]
}

function FilterCollection() {
    const [appliedFiltersIds, setAppliedFiltersIds] = useState<number[]>([]) // TODO Figure out how to not pass the setter all the way down to the button

    const removeFromAppliedFilters = (id: number) => {
        setAppliedFiltersIds(appliedFiltersIds.filter((filterId) => filterId !== id))
    }

    const addToAppliedFilters = (id: number) => {
        setAppliedFiltersIds([...appliedFiltersIds, id])
    }

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
                        appliedFiltersIds={appliedFiltersIds}
                        datasetSignals={["voltage", "current"]}
                        onApplyFilter={appliedFiltersIds.includes(filter.id) ? removeFromAppliedFilters : addToAppliedFilters}
                    />)
                }
            </ScrollableContainer>
        </DropShadowContainer>
    )
}

export default FilterCollection;