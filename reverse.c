#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int args, char *argv[]) {
	char A[100];
	int counter = 0;
	int f = open("file_to_read.txt", O_RDONLY);
	if (f < 0){
		write(1, "opening file failed!\n", 21);
		return -1;
	}
	char* end = A;

	while(1){
		char temp;
		int n = read(f, &temp, 1);
		if (n < 0){
			write(1, "reading file failed!\n", 21);
			return -1;
		}
		if (temp=='\n'){
			A[counter] = '\n';
			close(f);
			break;
		}
		end++;
		A[counter++] = (char) temp;
	}
	
	char B[counter];

	for (int i=0; i < counter; i++){
		B[i] =  *(end - i - 1);
	}

	B[counter]= '\n'; 

	pid_t child_pid = fork();

	if (child_pid < 0){
		write(1, "fork failed!\n", 13);
		return -1;
	} else if (child_pid == 0) {
		write(1, "child process started!\n", 23);
		int f_reverse = open("file_to_write.txt", O_CREAT | O_WRONLY | O_TRUNC, 0642);
		if (f_reverse < 0){
			write(1, "opening file failed!\n", 21);
			return -1;
		}
		write(f_reverse, B, counter+1);
		write(1, "writed reverse!\n", 16);
		close(f_reverse);
		write(1, "child process ended!\n", 21);
	} else if (child_pid > 0){
		int status;
		pid_t waited_pid = waitpid(child_pid, &status, 0);
		
		if (waited_pid < 0){
			write(1, "waitpid() failed!\n", 18);
			return -1;
		} else if (waited_pid == child_pid) {
			write(1, "parent process started!\n", 24); 
			int f_normal = open("file_to_write.txt", O_APPEND | O_WRONLY, 0642);
			write(f_normal, A, counter+1);
			write(1, "writed normal!\n", 15);
			close(f_normal);
			write(1, "parent process ended!\n", 22);
		}
	}

	return 0;
}
