import {FaRegCircleCheck, FaRegCircleXmark} from "react-icons/fa6";

interface ApprovalIconProps {
    approved: boolean;
    color?: string;
}

function ApprovalIcon({approved, color}: ApprovalIconProps) {
    if (approved) return <FaRegCircleCheck color={color}/>
    return <FaRegCircleXmark opacity={0.4} color={color}/>

}

export default ApprovalIcon;