import mockData from "@/components/data/mock-data/MockData.ts";
import Chart from "@/components/dashboard/chart/Chart.tsx";
import ChHeader from "@/components/dashboard/chart/header/ChHeader.tsx";
import {VStack} from "@chakra-ui/react";

function ChartWidget() {
    return (
        <VStack gap={3} height={"100%"} overflow="hidden">
            <ChHeader unit1={"Unit 1"} unit2={"Unit 2"}/>
            <Chart data={mockData.chartData} axisConfig={{right: ["pv"], left: ["uv", "amt"]}}/>
        </VStack>
    )
}

export default ChartWidget;