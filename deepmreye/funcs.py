import os
import warnings
import numpy as np
import pandas as pd
import plotly.graph_objs as go
from plotly.subplots import make_subplots
from os.path import join

def visualise_predictions_slider(
    evaluation,
    scores,
    color="rgb(0, 150, 175)",
    bg_color="rgb(247,247,247)",
    line_color="rgb(240,240,240)",
    ylim=None,
    subTR=False,
):
    """
    Visualize per-participant gaze predictions with an optional summary panel.

    - If `scores` is empty ({}), the left boxplot panel is omitted and only the
      time-series slider is shown.
    - If `scores` is provided, the left panel shows Pearson/R^2 across participants
      for the 4 variants present in your score tables.
    """
    if ylim is None:
        ylim = [-6, 6]

    no_scores = (not scores)
    participants = list(evaluation.keys())

    # ---- Build scores (only if provided) ----
    if not no_scores:
        all_scores = [df.values for df in scores.values()]
        all_scores = np.array(all_scores)
        # Expect shape (..., 6) or similar; original code plots columns 2 and 5 (Pearson Mean, R2 Mean)
        to_plot = np.concatenate((all_scores[..., 2], all_scores[..., 5]), axis=0)
        x = ["Pearson"] * all_scores[..., 2].shape[0] + ["R^2-Score"] * all_scores[..., 5].shape[0]
        hover_texts = []
        for subj in participants * 2:
            this_sub = os.path.splitext(os.path.basename(subj))[0]
            hover_texts.append(f"participant {this_sub}")

    # ---- Layout: dynamic grid based on whether scores exist ----
    if no_scores:
        # No scores: single column layout (time-series spans full width)
        fig = go.Figure(
            make_subplots(
                rows=2,
                cols=1,
                vertical_spacing=0.15,
                shared_xaxes=True,
                specs=[[{}], [{}]],
            )
        )
    else:
        # With scores: 2x4 grid (original layout)
        fig = go.Figure(
            make_subplots(
                rows=2,
                cols=4,
                horizontal_spacing=0.05,
                vertical_spacing=0.15,
                shared_xaxes="columns",
                specs=[[{"rowspan": 2, "colspan": 2}, None, {"colspan": 2}, None],
                       [None, None, {"colspan": 2}, None]],
            )
        )

    # Left boxplot panel (only if scores exist)
    if not no_scores:
        fig.add_trace(
            go.Box(
                y=to_plot[:, 0], x=x, marker_size=12, boxpoints="all", pointpos=0,
                marker=dict(opacity=0.65, color=color, line=dict(color="rgb(0,0,0)", width=2)),
                name="Default", text=hover_texts, line=dict(color="rgb(0,0,0)"),
                fillcolor="rgb(180, 180, 180)",
            ),
            row=1, col=1,
        )
        fig.add_trace(
            go.Box(
                y=to_plot[:, 1], x=x, marker_size=12, boxpoints="all", pointpos=0,
                marker=dict(opacity=0.65, color=color, line=dict(color="rgb(0,0,0)", width=2)),
                name="Default subTR", text=hover_texts, line=dict(color="rgb(0,0,0)"),
                fillcolor="rgb(180, 180, 180)",
            ),
            row=1, col=1,
        )
        fig.add_trace(
            go.Box(
                y=to_plot[:, 2], x=x, marker_size=12, boxpoints="all", pointpos=0,
                marker=dict(opacity=0.65, color=color, line=dict(color="rgb(0,0,0)", width=2)),
                name="Refined", text=hover_texts, line=dict(color="rgb(0,0,0)"),
                fillcolor="rgb(180, 180, 180)",
            ),
            row=1, col=1,
        )
        fig.add_trace(
            go.Box(
                y=to_plot[:, 3], x=x, marker_size=12, boxpoints="all", pointpos=0,
                marker=dict(opacity=0.65, color=color, line=dict(color="rgb(0,0,0)", width=2)),
                name="Refined subTR", text=hover_texts, line=dict(color="rgb(0,0,0)"),
                fillcolor="rgb(180, 180, 180)",
            ),
            row=1, col=1,
        )

    # ---- Time-series panel ----
    for key, item in evaluation.items():
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            if subTR:
                this_real = np.reshape(item["real_y"], (item["real_y"].shape[0] * item["real_y"].shape[1], -1))
                this_pred = np.reshape(item["pred_y"], (item["pred_y"].shape[0] * item["pred_y"].shape[1], -1))
            else:
                this_real = np.nanmedian(item["real_y"], axis=1)
                this_pred = np.nanmedian(item["pred_y"], axis=1)
        this_sub = os.path.splitext(os.path.basename(key))[0]

        # Determine column position based on layout
        col_pos = 1 if no_scores else 3

        # X component
        fig.add_trace(
            go.Scatter(
                x=np.arange(0, len(this_real[:, 0])),
                y=this_real[:, 0],
                mode="lines",
                visible=False,
                line_color="rgb(0,0,0)",
                opacity=0.5,
                line_width=3,
                name=this_sub,
            ),
            row=1, col=col_pos,
        )
        fig.add_trace(
            go.Scatter(
                x=np.arange(0, len(this_pred[:, 0])),
                y=this_pred[:, 0],
                mode="lines",
                visible=False,
                line_color=color,
                opacity=0.85,
                line_width=3,
                name=this_sub,
            ),
            row=1, col=col_pos,
        )
        # Y component
        fig.add_trace(
            go.Scatter(
                x=np.arange(0, len(this_real[:, 1])),
                y=this_real[:, 1],
                mode="lines",
                visible=False,
                line_color="rgb(0,0,0)",
                opacity=0.5,
                line_width=3,
                name=this_sub,
            ),
            row=2, col=col_pos,
        )
        fig.add_trace(
            go.Scatter(
                x=np.arange(0, len(this_pred[:, 1])),
                y=this_pred[:, 1],
                mode="lines",
                visible=False,
                line_color=color,
                opacity=0.85,
                line_width=3,
                name=this_sub,
            ),
            row=2, col=col_pos,
        )

    # Make the first participant visible
    offset = 0 if no_scores else 4
    if len(fig.data) >= offset + 4:
        for i in range(4):
            fig.data[offset + i].visible = True

    # ---- Slider (one step per participant) ----
    steps, stepcount = [], 0
    for i in range(offset, len(fig.data), 4):
        if stepcount >= len(participants):
            break
        this_sub = os.path.splitext(os.path.basename(participants[stepcount]))[0]
        base_visible = ([True] * 4 + [False] * (len(fig.data) - 4)) if not no_scores else ([False] * len(fig.data))
        step = dict(method="update", label=f"{this_sub}", args=[{"visible": base_visible}])
        for j in range(4):
            if i + j < len(fig.data):
                step["args"][0]["visible"][i + j] = True
        steps.append(step)
        stepcount += 1

    sliders = [dict(active=0, currentvalue={"prefix": "", "visible": False}, pad={"t": 70, "b": 10}, steps=steps)]

    # ---- Titles / layout ----
    annotations = []
    if not no_scores:
        annotations.append(dict(
            x=0.10, y=1.08, xref="paper", yref="paper",
            text="<b>Model Performance across participants</b>",
            font=dict(size=20), showarrow=False,
        ))
        annotations.append(dict(
            x=0.855, y=1.08, xref="paper", yref="paper",
            text="<b>Predicted vs. True gaze position</b>",
            font=dict(size=20), showarrow=False,
        ))
    else:
        annotations.append(dict(
            x=0.5, y=1.08, xref="paper", yref="paper",
            text="<b>Predicted gaze position</b>",
            font=dict(size=20), showarrow=False,
        ))

    fig.update_layout(
        showlegend=False,
        margin=dict(t=70, l=50, b=50, r=50),
        plot_bgcolor=bg_color,
        paper_bgcolor=bg_color,
        boxmode="group",
        autosize=False,
        width=1600 if no_scores else 1600,  # Same width, but different distribution
        height=650,
        annotations=annotations,
        sliders=sliders,
    )

    # Left panel axes only if scores exist
    if not no_scores:
        fig.update_yaxes(range=[-1.1, 1.1], linecolor=line_color, zerolinecolor=line_color,
                         gridcolor=line_color, row=1, col=1)
        fig.update_xaxes(tickfont=dict(size=20), row=1, col=1)

    # Time-series axes
    row1_col = 1 if no_scores else 3
    fig.update_yaxes(
        range=ylim, row=1, col=row1_col,
        linecolor=line_color, zerolinecolor=line_color, gridcolor=line_color,
        ticksuffix="°", title=dict(text="X", standoff=0, font=dict(size=20)),
    )
    fig.update_yaxes(
        range=ylim, row=2, col=1 if no_scores else 3,
        linecolor=line_color, zerolinecolor=line_color, gridcolor=line_color,
        ticksuffix="°", title=dict(text="Y", standoff=0, font=dict(size=20)),
    )

    # X ranges
    x_range = 150
    x_start = -2
    if subTR and evaluation:
        # scale window by inner timesteps (use the first entry for shape)
        first_item = next(iter(evaluation.values()))
        x_start = 10000
        x_range *= first_item["real_y"].shape[1]

    fig.update_xaxes(
        range=[x_start, x_range + x_start],
        row=1, col=row1_col,
        title=dict(text="Functional Volume (TR)", standoff=16, font=dict(size=20)),
    )
    fig.update_xaxes(range=[x_start, x_range + x_start], row=2, col=1 if no_scores else 3)

    return fig


def load_label(label_path, label_type="calibration_run", calibration=True):
    """Load label for experiment, which should return X,Y coordinates for each timepoint.

    This function can be exchanged for experiment specific loading of labels,
    or by using different label types.

    Parameters
    ----------
    label_path : str
        Path to file with labels
    label_type : str, optional
        Which type of labels are used in the experiment, by default 'calibration_run'

    Returns
    -------
    this_label : numpy array
        X,Y coordinates for each functional describing gaze position during this timepoint.
    """
    if label_type == "calibration_run":
        # Load labels from file
        labels = np.genfromtxt(label_path, delimiter=",")
        labels = labels[1:]
        #labels = np.repeat(labels, 5, axis=0) GOT RID OF THIS
        this_label = labels[:, np.newaxis, :]
        #this_label = np.repeat(this_label, 10, axis=1) GOT RID OF THIS

        # Normalize label
        this_label = (this_label - -0.95) / (0.95 - -0.95)
        this_label -= 0.5

        # Y-axis is flipped for this dataset
        this_label[..., 1] *= -1

        # Convert to visual angles
        this_label[..., 0] *= 19
        this_label[..., 1] *= 14.7

    return this_label

def process_one_subject_session(subject, session, calibration):
    # Load masks once
    eyemask_small, eyemask_big, dme_template, mask, x_edges, y_edges, z_edges = get_masks()

    # Dictionary to track processing results
    processing_results = {
        'success': [],
        'failed': [],
        'skipped': []
    }

    data_key = f"{subject}_{session}"
    subject_path = os.path.join(calibration_path, subject)
    nii_path = os.path.join(subject_path, session, f"{data_key}_calibration.nii")

    if not os.path.exists(nii_path):
        print(f"NIfTI file missing: {nii_path}")
        processing_results['skipped'].append(data_key)
        return processing_results

    try:
        # 1) Load labels
        if calibration == True:
            labels = load_label(label_path=label_path, label_type="calibration_run", calibration=True)
        else:
            labels = load_label(label_path=label_path, label_type="calibration_run", calibration=False)
            

        # 2) Process NIfTI
        masked_eye_data, transform_stats = run_participant(
            nii_path, dme_template, eyemask_big,
            eyemask_small, x_edges, y_edges, z_edges
        )
        masked_eye_data = normalize_img(masked_eye_data)

        # 3) Sanity checks
        if masked_eye_data.ndim != 4:
            raise ValueError(f"Expected 4D data, got {masked_eye_data.ndim}D")
        if labels.shape[0] != masked_eye_data.shape[3]:
            raise ValueError(
                f"Label mismatch ({labels.shape[0]} vs {masked_eye_data.shape[3]} volumes)"
            )

        # 4) Save
        T = labels.shape[0]
        ids_this_run = np.repeat(data_key, T)[None, :]
        if calibration == True:
            save_data(
                participant=f"{data_key}.npz",
                participant_data=[masked_eye_data],
                participant_labels=[labels],
                participant_ids=[ids_this_run],
                processed_data=calibration_npz
            )
        else:
            save_data(
                participant=f"{data_key}.npz",
                participant_data=[masked_eye_data],
                participant_labels=[labels],
                participant_ids=[ids_this_run],
                processed_data=movie_npz
            )

        processing_results['success'].append(data_key)
        print(f"Successfully processed {data_key}")

    except Exception as e:
        print(f"Error processing {data_key}: {e}")
        traceback.print_exc()
        processing_results['failed'].append(data_key)

    return processing_results

def process_all_subjects(calibration):
    # Get list of all subjects
    subjects = sorted([d for d in os.listdir(calibration_path) 
                      if os.path.isdir(os.path.join(calibration_path, d)) and d.startswith('sub-')])

    # Load masks once
    eyemask_small, eyemask_big, dme_template, mask, x_edges, y_edges, z_edges = get_masks()

    # Dictionary to track processing results
    processing_results = {
        'success': [],
        'failed': [],
        'skipped': []
    }

    for subject in subjects:
        subject_path = os.path.join(calibration_path, subject)
        sessions = [d for d in os.listdir(subject_path) 
                   if os.path.isdir(os.path.join(subject_path, d)) and d.startswith('ses-')]
        
        for session in sessions:
            data_key = f"{subject}_{session}"
            nii_path = os.path.join(subject_path, session, f"{data_key}_calibration.nii")
            
            if not os.path.exists(nii_path):
                print(f"NIfTI file missing: {nii_path}")
                processing_results['skipped'].append(data_key)
                continue

            try:
                # 1) Load labels
                if calibration == True:
                    labels = load_label(label_path=calibration_labels, label_type="calibration_run", calibration=True)
                else:
                    labels = load_label(label_path=movie_labels, label_type="movie_run", calibration=False)
            
                # 2) Process NIfTI
                masked_eye_data, transform_stats = run_participant(
                    nii_path, dme_template, eyemask_big,
                    eyemask_small, x_edges, y_edges, z_edges
                )
                masked_eye_data = normalize_img(masked_eye_data)
            
                # 3) Sanity checks
                if masked_eye_data.ndim != 4:
                    raise ValueError(f"Expected 4D data, got {masked_eye_data.ndim}D")
                if labels.shape[0] != masked_eye_data.shape[3]:
                    raise ValueError(
                        f"Label mismatch ({labels.shape[0]} vs {masked_eye_data.shape[3]} volumes)"
                    )
            
                # 4) Save
                T = labels.shape[0]
                ids_this_run = np.repeat(data_key, T)[None, :]
                
                os.makedirs(calibration_npz, exist_ok=True)

                if calibration == True:
                    save_data(
                        participant=f"{data_key}.npz",
                        participant_data=[masked_eye_data],
                        participant_labels=[labels],
                        participant_ids=[ids_this_run],
                        processed_data=calibration_npz
                    )
                else:
                    save_data(
                        participant=f"{data_key}.npz",
                        participant_data=[masked_eye_data],
                        participant_labels=[labels],
                        participant_ids=[ids_this_run],
                        processed_data=movie_npz
                    )
            
                processing_results['success'].append(data_key)
                print(f"Successfully processed {data_key}")
            
            except Exception as e:
                print(f"Error processing {data_key}: {e}")
                processing_results['failed'].append(data_key)


    # Print summary
    print("\nProcessing Summary:")
    print(f"Successfully processed: {len(processing_results['success'])}")
    print(f"Failed: {len(processing_results['failed'])}")
    print(f"Skipped: {len(processing_results['skipped'])}")
    
    return processing_results


def save_test_data(participant, participant_data, participant_ids, processed_data):
    """Save test participant data to npz file (without labels).

    Parameters
    ----------
    participant : str
        Participant label
    participant_data : list
        4D (X,Y,Z,t) data for participant across runs
    participant_ids : str
        Participant identifier with run id
    processed_data : str
        Filepath to where processed data should be stored
    """
    # Process data
    participant_data = np.transpose(np.concatenate(participant_data, axis=3), axes=(3, 0, 1, 2))
    participant_ids = np.concatenate(participant_ids, axis=1).transpose()

    # Save data in npz file (lazy loading)
    data_dict = {}
    for idx, (data, identifier) in enumerate(zip(participant_data, participant_ids)):
        data_dict[f"data_{idx}"] = data
        data_dict[f"identifier_{idx}"] = identifier

    # Save each subject in separate .npz files
    subject_file_path = join(processed_data, participant)
    print(
        f"Saving test eye data {participant_data.shape} "
        f"(no labels) to file {subject_file_path}"
    )
    
    np.savez(subject_file_path, **data_dict)

def get_test_subject_data(fn_subject):
    """Get subject data for test files (without labels)"""
    data = np.load(fn_subject, mmap_mode="r")
    
    # Count how many data samples we have
    data_keys = [key for key in data.files if key.startswith('data_')]
    num_trs = len(data_keys)
    
    subject_data_X = []
    for sample_index in range(num_trs):
        subject_data_X.append(data[f"data_{str(sample_index)}"])
    
    subject_data_X = np.array(subject_data_X)
    return subject_data_X[..., np.newaxis]  # Add channel dimension

