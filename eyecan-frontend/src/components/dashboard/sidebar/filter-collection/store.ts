import {create} from "zustand/react";

interface SelectedFiltersStore {
    filterIds: string[];
    removeFilterId: (id: string) => void;
    addFilterId: (id: string) => void;
}

const useSelectedFiltersStore = create<SelectedFiltersStore>(
    (set) => ({
    filterIds: [],
    removeFilterId: (id) => set((state) => ({filterIds: state.filterIds.filter((filterId) => filterId !== id)})),
    addFilterId: (id) => set((state) => ({filterIds: [...state.filterIds, id]}))
}))


export default useSelectedFiltersStore;