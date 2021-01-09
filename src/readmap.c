#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int		**free_map(int fd, int **map, int opt)
{
	int		i;

	if (opt == 1)
		close(fd);
	i = 0;
	if (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
	return (NULL);
}

int		*read_line(int fd, int line)
{
	int		*cas;
	char	buff[25];
	int		i;

	i = -1;
	if (!(cas = (int *)(malloc(sizeof(int) * 25))))
		return (NULL);
	if ((read(fd, buff, 25)) != 25 || buff[24] != '\n')
	{
		free(cas);
		return (NULL);
	}
	while (++i < 24)
	{
		if (buff[i] < '0' || buff[i] > '9' || (buff[i] < '4' && (i == 0 || i == 23 || line == 0 || line == 23)))
		{
			free(cas);
			return (NULL);
		}
		cas[i] = buff[i] - '0';
	}
	return (cas);
}

int		**read_map(char *path)
{
	int		fd;
	int		**map;
	int		i;
	char	buff[1];

	if ((fd = open(path, O_RDONLY)) == -1)
		return (NULL);
	map = NULL;
	if (!(map = (int **)(malloc(sizeof(int *) * (25)))))
		return (NULL);
	map[24] = NULL;
	i = -1;
	while (++i < 24)
	{
		if (!(map[i] = read_line(fd, i)))
			return (free_map(fd, map, 1));
	}
	if (read(fd, buff, 1) != 0)
		return (free_map(fd, map, 1));
	return (map);
}