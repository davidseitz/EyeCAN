import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts';
import mockData from "../../data/mock-data/MockData.ts"
import {useColorModeValue} from "@/components/ui/color-mode.tsx";
import style from './chart.module.css'

function Chart() {
    const axisStrokeColor = useColorModeValue('#000000', '#ffffff');
    const axisStyle = {stroke: axisStrokeColor, strokeWidth: 2}
    const gridColor = '#ccc';
    const tickStyle = {
        fill: axisStrokeColor
    };


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
                <Line yAxisId="left" type="monotone" dataKey="pv" stroke="#8884d8"/>
                <Line yAxisId="right" type="monotone" dataKey="uv" stroke="#82ca9d"/>
                <Line yAxisId="right" type="monotone" dataKey="amt" stroke="#82cef4"/>
            </LineChart>
        </ResponsiveContainer>
    );
}

export default Chart;
