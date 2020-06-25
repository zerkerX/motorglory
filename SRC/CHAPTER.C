#include "chapter.h"
#include "stage.h"
#include "stage.h"
#include "game.h"
#include "sound.h"
#include "moptions.h"

#include <i86.h>
#include <stdlib.h>
#include <string.h>

/* Only one chapter can exist at a time */
struct chapter curr_chapter = {NULL, NULL, 0, 0, ""};


/* NOTE: Stages are numbered starting from 1 */
struct stage * chapter_get_stage_num(int16_t num)
{
    struct stage * result = NULL;

    struct stage * stage;
    int stagenum = 1;
    for (stage = curr_chapter.first_stage; stage != NULL;
        stage = stage->next )
    {
        if (num == stagenum)
        {
            result = stage;
            break;
        }
        ++stagenum;
    }

    return result;
}


void chapter_run()
{
    if (settings.dblbuff) video_enable_double_buffer();
    actor_load_sprites();
    stage_load_tiles();
    game_state.mode = MODE_PLAY;
    game_state.next_stage = curr_chapter.start_stage_num;
    game_state.next_x = -1;
    game_state.next_y = -1;
    while (game_state.mode == MODE_PLAY || game_state.mode == MODE_NEWSTAGE)
    {
        stage_run(chapter_get_stage_num(game_state.next_stage));
    }
    actor_unload_sprites();
    stage_unload_tiles();
    if (settings.dblbuff) video_disable_double_buffer();
}


void chapter_new()
{
    if (curr_chapter.first_stage != NULL)
        chapter_unload();
    
    /* Create a single stage */
    chapter_add_new();
    curr_chapter.start_stage_num = 1;
    strcpy(curr_chapter.filename, "UNTITLED.MAP");
}


void chapter_add_new()
{
    struct stage * newstage = malloc(sizeof(struct stage));
    stage_new(newstage);
    if (curr_chapter.last_stage == NULL)
    {
        /* First stage created */
        curr_chapter.last_stage = newstage;
        curr_chapter.first_stage = newstage;
    }
    else
    {
        /* After previous last */
        curr_chapter.last_stage->next = newstage;
        newstage->prev = curr_chapter.last_stage;
    }
    curr_chapter.last_stage = newstage;
    curr_chapter.num_stages++;
}


void chapter_remove(struct stage * remstage)
{
    if (remstage == curr_chapter.first_stage)
        curr_chapter.first_stage = remstage->next;
    else
        remstage->prev->next = remstage->next;

    if (remstage == curr_chapter.last_stage)
        curr_chapter.last_stage = remstage->prev;
    else
        remstage->next->prev = remstage->prev;

    curr_chapter.num_stages--;
    stage_free(remstage);
    
    /* Don't allow zero stages. Create a blank one now */
    if (curr_chapter.num_stages <= 0)
        chapter_add_new();
}


void chapter_load_from(FILE * input)
{
    int i;
    struct stage * stage = NULL;
    struct stage * prev_stage = NULL;
    
    if (curr_chapter.first_stage != NULL)
        chapter_unload();

    /* Load the main chapter data */
    fread(&curr_chapter, sizeof(curr_chapter), 1, input);

    /* Fix the starting stage for legacy maps */
    if (curr_chapter.start_stage_num < 1)
        curr_chapter.start_stage_num = 1;

    /* Load all stages */
    for (i = 0; i < curr_chapter.num_stages; ++i )
    {
        stage = malloc(sizeof(*stage));
        stage_load(stage, input);
        if (prev_stage != NULL)
        {
            prev_stage->next = stage;
            stage->prev = prev_stage;
        }
        else
        {
            /* First stage */
            curr_chapter.first_stage = stage;
            stage->prev = NULL;
        }
        prev_stage = stage;
    }
    curr_chapter.last_stage = prev_stage;
}


void chapter_load(const char * filename)
{
    /* TODO Error checking */
    char fullpath[30];
    FILE * input;

    sprintf(fullpath, "levels\\%s", filename);

    input = fopen(fullpath, "rb");
    chapter_load_from(input);

    fclose(input);
}


void chapter_save_into(FILE * output)
{
    struct stage * stage;

    /* Save the main chapter data */
    fwrite(&curr_chapter, sizeof(curr_chapter), 1, output);

    /* Save all stages */
    for (stage = curr_chapter.first_stage; stage != NULL;
        stage = stage->next )
    {
        stage_save(stage, output);
    }
}


void chapter_save()
{
    char fullpath[30];
    FILE * output;

    /* TODO: Error checking */

    sprintf(fullpath, "levels\\%s", curr_chapter.filename);
    output = fopen(fullpath, "wb");
    chapter_save_into(output);
    fclose(output);
}


void chapter_save_as(const char * filename)
{
    strcpy(curr_chapter.filename, filename);
    chapter_save();
}


void chapter_unload()
{
    struct stage * stage;
    struct stage * next_stage;

    /* Free all stages */
    for (stage = curr_chapter.first_stage; stage != NULL;
        stage = next_stage )
    {
        next_stage = stage->next;
        stage_free(stage);
    }
    curr_chapter.first_stage = NULL;
    curr_chapter.last_stage = NULL;
    curr_chapter.num_stages = 0;
    curr_chapter.start_stage_num = 0;
    strcpy(curr_chapter.filename, "");
}
