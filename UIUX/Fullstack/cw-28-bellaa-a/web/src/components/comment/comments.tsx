import Comment from "./comment";
import useQueryComments from "@/hooks/use-query-comments";

const comments = ({ postId }: { postId: string }) => {
  const { comments } = useQueryComments(postId);

  return (
    <div className="">
      {comments.map((comment) => (
        <Comment comment={comment} key={comment.id} />
      ))}
    </div>
  );
};

export default comments;
