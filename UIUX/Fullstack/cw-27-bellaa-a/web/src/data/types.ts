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

export type UserType = {
  id: string;
  name: string;
  username: string;
};