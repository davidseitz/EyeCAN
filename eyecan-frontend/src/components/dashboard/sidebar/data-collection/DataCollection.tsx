import mockData from "@/data/MockData.ts";
import DataCollectionCard from "@/components/dashboard/sidebar/data-collection/DataCollectionCard.tsx";
import {useState} from "react";
import {Dataset} from "@/types/dataset.ts";

function DataCollection() {
    const [datasets, setDatasets] = useState<Dataset[]>(mockData.datasets) // TODO How to share this in whole application
    const [selectedDatasetIndex, setSelectedDatasetIndex] = useState(0)
    const threshold = mockData.datasets.length - 1;

    const handleDatasetChange = (increase: boolean) => {
        setSelectedDatasetIndex((prevIndex) => {
            let newIndex = increase ? prevIndex + 1 : prevIndex - 1;
            if (newIndex > threshold) newIndex = 0;
            if (newIndex < 0) newIndex = threshold;

            return newIndex;
        });
    }

    return (
        <DataCollectionCard onNavigate={handleDatasetChange}
                            dataset={datasets[selectedDatasetIndex]}/>
    )
}

export default DataCollection;