import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import FilterCollection from "@/components/dashboard/sidebar/filter-collection/FilterCollection.tsx";
import DataCollectionCard from "@/components/dashboard/sidebar/data-collection/DataCollectionCard.tsx";

function SideBar() {
    return (
        <>
            <FilterCollection/>
            <DataCollectionCard/>
            <DropShadowContainer height={"15%"} />
        </>
    )
}

export default SideBar;