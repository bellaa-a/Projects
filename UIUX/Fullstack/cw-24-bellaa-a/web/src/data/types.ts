export type PostType = {
  id: string;
  content: string;
  date: string;
};

export type CommentType = {
  id: string;
  content: string;
  date: string;
  postId: string;
};
