import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts';
import {useColorModeValue} from "@/components/ui/color-mode.tsx";
import {hex} from 'generate-random-color';
import style from './chart.module.css'

export interface AxisConfig {
    left: string[];
    right: string[];
}

interface ChartProps {
    axisConfig: AxisConfig;
    data: object[];
}

interface ColorModeProperties {
    axisStrokeColor: string;
    contentStyle: ContentStyle;
    labelStyle: LabelStyle;
}

interface ContentStyle {
    backgroundColor: string;
    border: string;
    color: string;
    borderRadius: number;
    boxShadow: string;
}

interface LabelStyle {
    color: string;
}

function Chart({axisConfig, data}: ChartProps) {
    const colorProperties = useColorModeValue<ColorModeProperties>({
        axisStrokeColor: '#818181',
        contentStyle: {backgroundColor: 'white', border: 'none', color: 'black', borderRadius: 5, boxShadow: '0px 0px 10px 0px rgba(140, 140, 140, 0.5)'},
        labelStyle: {color: 'black'}
    }, {
        axisStrokeColor: '#ffffff',
        contentStyle: {backgroundColor: 'black', border: 'none', color: 'white', borderRadius: 5, boxShadow: '0px 0px 5px 0px rgba(140, 140, 140, 0.5)'},
        labelStyle: {color: 'white'}
    })

//     const axisStrokeColor = useColorModeValue('#818181', '#ffffff');
    const axisStyle = {stroke: colorProperties.axisStrokeColor, strokeWidth: 2}
    const gridColor = '#ccc';
    const tickStyle = {fill: colorProperties.axisStrokeColor};
    const keys = Object.keys(data[0]).filter(key => key !== 'time');

    return (
        <ResponsiveContainer className={style.chart} width="100%" height="86%">
            <LineChart margin={{top: 2, right: -20, left: -20}}
                       data={data}>
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
                <Tooltip
                    contentStyle={colorProperties.contentStyle}
                    labelStyle={{...colorProperties.labelStyle, fontWeight: 'bold', fontSize: 12}}
                    itemStyle={{fontWeight: 'bold', fontSize: 12}}
                    cursor={{ stroke: '#a0a0a0', strokeWidth: 1 }}
                />
                <Legend />
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

