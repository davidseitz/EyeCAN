import FilterCollection from "@/components/dashboard/sidebar/filter-collection/FilterCollection.tsx";
import DataCollectionCard from "@/components/dashboard/sidebar/data-collection/DataCollectionCard.tsx";
import ActionButtons from "@/components/dashboard/sidebar/action-buttons/ActionButtons.tsx";

function SideBar() {
    return (
        <>
            <FilterCollection/>
            <DataCollectionCard/>
            <ActionButtons/>
        </>
    )
}

export default SideBar;