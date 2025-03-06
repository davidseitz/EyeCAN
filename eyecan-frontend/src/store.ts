import {Dataset} from "@/types/dataset.ts";
import {create} from "zustand/react";

interface SelectedDatasetStore {
    dataset: Dataset | null;
    setDataset: (dataset: Dataset) => void;
    resetDataset: () => void;
}

const useSelectedDatasetStore = create<SelectedDatasetStore>(
    (set) => ({
        dataset: null,
        setDataset: (dataset) => set({dataset}),
        resetDataset: () => set({dataset: null})
    })
)

export default useSelectedDatasetStore;