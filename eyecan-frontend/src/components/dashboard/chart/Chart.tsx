import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts';
import mockData from "../../data/mock-data/MockData.ts"
import {useColorModeValue} from "@/components/ui/color-mode.tsx";
import {hex} from 'generate-random-color';
import style from './chart.module.css'

export interface AxisConfig {
    left: string[];
    right: string[];
}

interface ChartProps {
    axisConfig: AxisConfig;
}

function Chart({axisConfig}: ChartProps) {
    const axisStrokeColor = useColorModeValue('#818181', '#ffffff');
    const axisStyle = {stroke: axisStrokeColor, strokeWidth: 2}
    const gridColor = '#ccc';
    const tickStyle = {fill: axisStrokeColor};
    const keys = Object.keys(mockData.chartData[0]).filter(key => key !== 'time');

    return (
        <ResponsiveContainer className={style.chart} width="100%" height="100%">
            <LineChart margin={{top: 2, right: -20, left: -20}}
                       data={mockData.chartData}>
                <CartesianGrid stroke={gridColor} strokeDasharray="3 3"/>
                <XAxis axisLine={axisStyle}
                       tick={tickStyle}
                       tickLine={axisStyle}
                       padding={{left: 0, right: 0}}
                       dataKey="time"/>
                <YAxis axisLine={axisStyle}
                       tick={tickStyle}
                       tickLine={axisStyle}
                       yAxisId="left"/>
                <YAxis axisLine={axisStyle}
                       tick={tickStyle}
                       tickLine={axisStyle}
                       yAxisId="right" orientation="right"/>
                <Tooltip/>
                <Legend/>
                {keys.map((key) => {
                    const yAxisId = axisConfig.left.includes(key)  ? 'left' : axisConfig.right.includes(key)  ? 'right' : null;
                    if (!yAxisId) return null;
                    return <Line yAxisId={yAxisId} type="monotone" key={key} dataKey={key} stroke={hex()}/>
                } )}
            </LineChart>
        </ResponsiveContainer>
    );
}

export default Chart;


//