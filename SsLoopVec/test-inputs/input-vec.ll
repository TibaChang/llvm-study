; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [18 x i8] c"\0A\0Aresult = [%d]\0A\0A\00", align 1

; Function Attrs: norecurse nounwind readonly uwtable
define dso_local i32 @foo(i32* nocapture readonly %A, i32 %n) local_unnamed_addr #0 !dbg !6 {
entry:
  %cmp6 = icmp sgt i32 %n, 0, !dbg !8
  br i1 %cmp6, label %for.body.preheader, label %for.cond.cleanup, !dbg !9

for.body.preheader:                               ; preds = %entry
  %wide.trip.count = zext i32 %n to i64, !dbg !8
  %min.iters.check = icmp ult i32 %n, 4, !dbg !9
  br i1 %min.iters.check, label %for.body.preheader11, label %vector.ph, !dbg !9

for.body.preheader11:                             ; preds = %middle.block, %for.body.preheader
  %indvars.iv.ph = phi i64 [ 0, %for.body.preheader ], [ %n.vec, %middle.block ]
  %b.07.ph = phi i32 [ 0, %for.body.preheader ], [ %31, %middle.block ]
  br label %for.body, !dbg !9

vector.ph:                                        ; preds = %for.body.preheader
  %n.vec = and i64 %wide.trip.count, 4294967292, !dbg !9
  %0 = add nsw i64 %n.vec, -4, !dbg !9
  %1 = lshr exact i64 %0, 2, !dbg !9
  %2 = add nuw nsw i64 %1, 1, !dbg !9
  %xtraiter = and i64 %2, 7, !dbg !9
  %3 = icmp ult i64 %0, 28, !dbg !9
  br i1 %3, label %middle.block.unr-lcssa, label %vector.ph.new, !dbg !9

vector.ph.new:                                    ; preds = %vector.ph
  %unroll_iter = sub nsw i64 %2, %xtraiter, !dbg !9
  br label %vector.body, !dbg !9

vector.body:                                      ; preds = %vector.body, %vector.ph.new
  %index = phi i64 [ 0, %vector.ph.new ], [ %index.next.7, %vector.body ], !dbg !10
  %vec.phi = phi <4 x i32> [ zeroinitializer, %vector.ph.new ], [ %27, %vector.body ]
  %niter = phi i64 [ %unroll_iter, %vector.ph.new ], [ %niter.nsub.7, %vector.body ]
  %4 = getelementptr inbounds i32, i32* %A, i64 %index, !dbg !11
  %5 = bitcast i32* %4 to <4 x i32>*, !dbg !11
  %wide.load = load <4 x i32>, <4 x i32>* %5, align 4, !dbg !11, !tbaa !12
  %6 = add <4 x i32> %wide.load, %vec.phi, !dbg !16
  %index.next = or i64 %index, 4, !dbg !10
  %7 = getelementptr inbounds i32, i32* %A, i64 %index.next, !dbg !11
  %8 = bitcast i32* %7 to <4 x i32>*, !dbg !11
  %wide.load.1 = load <4 x i32>, <4 x i32>* %8, align 4, !dbg !11, !tbaa !12
  %9 = add <4 x i32> %wide.load.1, %6, !dbg !16
  %index.next.1 = or i64 %index, 8, !dbg !10
  %10 = getelementptr inbounds i32, i32* %A, i64 %index.next.1, !dbg !11
  %11 = bitcast i32* %10 to <4 x i32>*, !dbg !11
  %wide.load.2 = load <4 x i32>, <4 x i32>* %11, align 4, !dbg !11, !tbaa !12
  %12 = add <4 x i32> %wide.load.2, %9, !dbg !16
  %index.next.2 = or i64 %index, 12, !dbg !10
  %13 = getelementptr inbounds i32, i32* %A, i64 %index.next.2, !dbg !11
  %14 = bitcast i32* %13 to <4 x i32>*, !dbg !11
  %wide.load.3 = load <4 x i32>, <4 x i32>* %14, align 4, !dbg !11, !tbaa !12
  %15 = add <4 x i32> %wide.load.3, %12, !dbg !16
  %index.next.3 = or i64 %index, 16, !dbg !10
  %16 = getelementptr inbounds i32, i32* %A, i64 %index.next.3, !dbg !11
  %17 = bitcast i32* %16 to <4 x i32>*, !dbg !11
  %wide.load.4 = load <4 x i32>, <4 x i32>* %17, align 4, !dbg !11, !tbaa !12
  %18 = add <4 x i32> %wide.load.4, %15, !dbg !16
  %index.next.4 = or i64 %index, 20, !dbg !10
  %19 = getelementptr inbounds i32, i32* %A, i64 %index.next.4, !dbg !11
  %20 = bitcast i32* %19 to <4 x i32>*, !dbg !11
  %wide.load.5 = load <4 x i32>, <4 x i32>* %20, align 4, !dbg !11, !tbaa !12
  %21 = add <4 x i32> %wide.load.5, %18, !dbg !16
  %index.next.5 = or i64 %index, 24, !dbg !10
  %22 = getelementptr inbounds i32, i32* %A, i64 %index.next.5, !dbg !11
  %23 = bitcast i32* %22 to <4 x i32>*, !dbg !11
  %wide.load.6 = load <4 x i32>, <4 x i32>* %23, align 4, !dbg !11, !tbaa !12
  %24 = add <4 x i32> %wide.load.6, %21, !dbg !16
  %index.next.6 = or i64 %index, 28, !dbg !10
  %25 = getelementptr inbounds i32, i32* %A, i64 %index.next.6, !dbg !11
  %26 = bitcast i32* %25 to <4 x i32>*, !dbg !11
  %wide.load.7 = load <4 x i32>, <4 x i32>* %26, align 4, !dbg !11, !tbaa !12
  %27 = add <4 x i32> %wide.load.7, %24, !dbg !16
  %index.next.7 = add i64 %index, 32, !dbg !10
  %niter.nsub.7 = add i64 %niter, -8, !dbg !10
  %niter.ncmp.7 = icmp eq i64 %niter.nsub.7, 0, !dbg !10
  br i1 %niter.ncmp.7, label %middle.block.unr-lcssa, label %vector.body, !dbg !10, !llvm.loop !17

middle.block.unr-lcssa:                           ; preds = %vector.body, %vector.ph
  %.lcssa.ph = phi <4 x i32> [ undef, %vector.ph ], [ %27, %vector.body ]
  %index.unr = phi i64 [ 0, %vector.ph ], [ %index.next.7, %vector.body ]
  %vec.phi.unr = phi <4 x i32> [ zeroinitializer, %vector.ph ], [ %27, %vector.body ]
  %lcmp.mod = icmp eq i64 %xtraiter, 0, !dbg !10
  br i1 %lcmp.mod, label %middle.block, label %vector.body.epil, !dbg !10

vector.body.epil:                                 ; preds = %middle.block.unr-lcssa, %vector.body.epil
  %index.epil = phi i64 [ %index.next.epil, %vector.body.epil ], [ %index.unr, %middle.block.unr-lcssa ], !dbg !10
  %vec.phi.epil = phi <4 x i32> [ %30, %vector.body.epil ], [ %vec.phi.unr, %middle.block.unr-lcssa ]
  %epil.iter = phi i64 [ %epil.iter.sub, %vector.body.epil ], [ %xtraiter, %middle.block.unr-lcssa ]
  %28 = getelementptr inbounds i32, i32* %A, i64 %index.epil, !dbg !11
  %29 = bitcast i32* %28 to <4 x i32>*, !dbg !11
  %wide.load.epil = load <4 x i32>, <4 x i32>* %29, align 4, !dbg !11, !tbaa !12
  %30 = add <4 x i32> %wide.load.epil, %vec.phi.epil, !dbg !16
  %index.next.epil = add i64 %index.epil, 4, !dbg !10
  %epil.iter.sub = add i64 %epil.iter, -1, !dbg !10
  %epil.iter.cmp = icmp eq i64 %epil.iter.sub, 0, !dbg !10
  br i1 %epil.iter.cmp, label %middle.block, label %vector.body.epil, !dbg !10, !llvm.loop !20

middle.block:                                     ; preds = %vector.body.epil, %middle.block.unr-lcssa
  %.lcssa = phi <4 x i32> [ %.lcssa.ph, %middle.block.unr-lcssa ], [ %30, %vector.body.epil ], !dbg !16
  %rdx.shuf = shufflevector <4 x i32> %.lcssa, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>, !dbg !9
  %bin.rdx = add <4 x i32> %.lcssa, %rdx.shuf, !dbg !9
  %rdx.shuf9 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>, !dbg !9
  %bin.rdx10 = add <4 x i32> %bin.rdx, %rdx.shuf9, !dbg !9
  %31 = extractelement <4 x i32> %bin.rdx10, i32 0, !dbg !9
  %cmp.n = icmp eq i64 %n.vec, %wide.trip.count, !dbg !9
  br i1 %cmp.n, label %for.cond.cleanup, label %for.body.preheader11, !dbg !9

for.cond.cleanup:                                 ; preds = %for.body, %middle.block, %entry
  %b.0.lcssa = phi i32 [ 0, %entry ], [ %31, %middle.block ], [ %add, %for.body ], !dbg !22
  ret i32 %b.0.lcssa, !dbg !23

for.body:                                         ; preds = %for.body.preheader11, %for.body
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body ], [ %indvars.iv.ph, %for.body.preheader11 ]
  %b.07 = phi i32 [ %add, %for.body ], [ %b.07.ph, %for.body.preheader11 ]
  %arrayidx = getelementptr inbounds i32, i32* %A, i64 %indvars.iv, !dbg !11
  %32 = load i32, i32* %arrayidx, align 4, !dbg !11, !tbaa !12
  %add = add nsw i32 %32, %b.07, !dbg !16
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1, !dbg !10
  %exitcond = icmp eq i64 %indvars.iv.next, %wide.trip.count, !dbg !8
  br i1 %exitcond, label %for.cond.cleanup, label %for.body, !dbg !9, !llvm.loop !24
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #2 !dbg !26 {
entry:
  %A = alloca [64 x i32], align 16
  %0 = bitcast [64 x i32]* %A to i8*, !dbg !27
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %0) #4, !dbg !27
  %1 = bitcast [64 x i32]* %A to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %1, align 16, !dbg !28, !tbaa !12
  %2 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 4, !dbg !29
  %3 = bitcast i32* %2 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %3, align 16, !dbg !28, !tbaa !12
  %4 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 8, !dbg !29
  %5 = bitcast i32* %4 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %5, align 16, !dbg !28, !tbaa !12
  %6 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 12, !dbg !29
  %7 = bitcast i32* %6 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %7, align 16, !dbg !28, !tbaa !12
  %8 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 16, !dbg !29
  %9 = bitcast i32* %8 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %9, align 16, !dbg !28, !tbaa !12
  %10 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 20, !dbg !29
  %11 = bitcast i32* %10 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %11, align 16, !dbg !28, !tbaa !12
  %12 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 24, !dbg !29
  %13 = bitcast i32* %12 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %13, align 16, !dbg !28, !tbaa !12
  %14 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 28, !dbg !29
  %15 = bitcast i32* %14 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %15, align 16, !dbg !28, !tbaa !12
  %16 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 32, !dbg !29
  %17 = bitcast i32* %16 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %17, align 16, !dbg !28, !tbaa !12
  %18 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 36, !dbg !29
  %19 = bitcast i32* %18 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %19, align 16, !dbg !28, !tbaa !12
  %20 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 40, !dbg !29
  %21 = bitcast i32* %20 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %21, align 16, !dbg !28, !tbaa !12
  %22 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 44, !dbg !29
  %23 = bitcast i32* %22 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %23, align 16, !dbg !28, !tbaa !12
  %24 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 48, !dbg !29
  %25 = bitcast i32* %24 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %25, align 16, !dbg !28, !tbaa !12
  %26 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 52, !dbg !29
  %27 = bitcast i32* %26 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %27, align 16, !dbg !28, !tbaa !12
  %28 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 56, !dbg !29
  %29 = bitcast i32* %28 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %29, align 16, !dbg !28, !tbaa !12
  %30 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 60, !dbg !29
  %31 = bitcast i32* %30 to <4 x i32>*, !dbg !28
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %31, align 16, !dbg !28, !tbaa !12
  %32 = bitcast [64 x i32]* %A to <4 x i32>*, !dbg !30
  %wide.load = load <4 x i32>, <4 x i32>* %32, align 16, !dbg !30, !tbaa !12
  %33 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 4, !dbg !30
  %34 = bitcast i32* %33 to <4 x i32>*, !dbg !30
  %wide.load.1 = load <4 x i32>, <4 x i32>* %34, align 16, !dbg !30, !tbaa !12
  %35 = add <4 x i32> %wide.load.1, %wide.load, !dbg !32
  %36 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 8, !dbg !30
  %37 = bitcast i32* %36 to <4 x i32>*, !dbg !30
  %wide.load.2 = load <4 x i32>, <4 x i32>* %37, align 16, !dbg !30, !tbaa !12
  %38 = add <4 x i32> %wide.load.2, %35, !dbg !32
  %39 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 12, !dbg !30
  %40 = bitcast i32* %39 to <4 x i32>*, !dbg !30
  %wide.load.3 = load <4 x i32>, <4 x i32>* %40, align 16, !dbg !30, !tbaa !12
  %41 = add <4 x i32> %wide.load.3, %38, !dbg !32
  %42 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 16, !dbg !30
  %43 = bitcast i32* %42 to <4 x i32>*, !dbg !30
  %wide.load.4 = load <4 x i32>, <4 x i32>* %43, align 16, !dbg !30, !tbaa !12
  %44 = add <4 x i32> %wide.load.4, %41, !dbg !32
  %45 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 20, !dbg !30
  %46 = bitcast i32* %45 to <4 x i32>*, !dbg !30
  %wide.load.5 = load <4 x i32>, <4 x i32>* %46, align 16, !dbg !30, !tbaa !12
  %47 = add <4 x i32> %wide.load.5, %44, !dbg !32
  %48 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 24, !dbg !30
  %49 = bitcast i32* %48 to <4 x i32>*, !dbg !30
  %wide.load.6 = load <4 x i32>, <4 x i32>* %49, align 16, !dbg !30, !tbaa !12
  %50 = add <4 x i32> %wide.load.6, %47, !dbg !32
  %51 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 28, !dbg !30
  %52 = bitcast i32* %51 to <4 x i32>*, !dbg !30
  %wide.load.7 = load <4 x i32>, <4 x i32>* %52, align 16, !dbg !30, !tbaa !12
  %53 = add <4 x i32> %wide.load.7, %50, !dbg !32
  %54 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 32, !dbg !30
  %55 = bitcast i32* %54 to <4 x i32>*, !dbg !30
  %wide.load.8 = load <4 x i32>, <4 x i32>* %55, align 16, !dbg !30, !tbaa !12
  %56 = add <4 x i32> %wide.load.8, %53, !dbg !32
  %57 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 36, !dbg !30
  %58 = bitcast i32* %57 to <4 x i32>*, !dbg !30
  %wide.load.9 = load <4 x i32>, <4 x i32>* %58, align 16, !dbg !30, !tbaa !12
  %59 = add <4 x i32> %wide.load.9, %56, !dbg !32
  %60 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 40, !dbg !30
  %61 = bitcast i32* %60 to <4 x i32>*, !dbg !30
  %wide.load.10 = load <4 x i32>, <4 x i32>* %61, align 16, !dbg !30, !tbaa !12
  %62 = add <4 x i32> %wide.load.10, %59, !dbg !32
  %63 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 44, !dbg !30
  %64 = bitcast i32* %63 to <4 x i32>*, !dbg !30
  %wide.load.11 = load <4 x i32>, <4 x i32>* %64, align 16, !dbg !30, !tbaa !12
  %65 = add <4 x i32> %wide.load.11, %62, !dbg !32
  %66 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 48, !dbg !30
  %67 = bitcast i32* %66 to <4 x i32>*, !dbg !30
  %wide.load.12 = load <4 x i32>, <4 x i32>* %67, align 16, !dbg !30, !tbaa !12
  %68 = add <4 x i32> %wide.load.12, %65, !dbg !32
  %69 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 52, !dbg !30
  %70 = bitcast i32* %69 to <4 x i32>*, !dbg !30
  %wide.load.13 = load <4 x i32>, <4 x i32>* %70, align 16, !dbg !30, !tbaa !12
  %71 = add <4 x i32> %wide.load.13, %68, !dbg !32
  %72 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 56, !dbg !30
  %73 = bitcast i32* %72 to <4 x i32>*, !dbg !30
  %wide.load.14 = load <4 x i32>, <4 x i32>* %73, align 16, !dbg !30, !tbaa !12
  %74 = add <4 x i32> %wide.load.14, %71, !dbg !32
  %75 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 60, !dbg !30
  %76 = bitcast i32* %75 to <4 x i32>*, !dbg !30
  %wide.load.15 = load <4 x i32>, <4 x i32>* %76, align 16, !dbg !30, !tbaa !12
  %77 = add <4 x i32> %wide.load.15, %74, !dbg !32
  %rdx.shuf = shufflevector <4 x i32> %77, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>, !dbg !33
  %bin.rdx = add <4 x i32> %77, %rdx.shuf, !dbg !33
  %rdx.shuf19 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>, !dbg !33
  %bin.rdx20 = add <4 x i32> %bin.rdx, %rdx.shuf19, !dbg !33
  %78 = extractelement <4 x i32> %bin.rdx20, i32 0, !dbg !33
  %call1 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i64 0, i64 0), i32 %78), !dbg !34
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %0) #4, !dbg !35
  ret i32 0, !dbg !36
}

; Function Attrs: nofree nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

attributes #0 = { norecurse nounwind readonly uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}
!llvm.ident = !{!5}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "input.c", directory: "/home/tiba/workspace/ssOptimizer/SsLoopVec/test-inputs")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
!6 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 2, type: !7, scopeLine: 2, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 5, column: 21, scope: !6)
!9 = !DILocation(line: 5, column: 3, scope: !6)
!10 = !DILocation(line: 5, column: 26, scope: !6)
!11 = !DILocation(line: 6, column: 10, scope: !6)
!12 = !{!13, !13, i64 0}
!13 = !{!"int", !14, i64 0}
!14 = !{!"omnipotent char", !15, i64 0}
!15 = !{!"Simple C/C++ TBAA"}
!16 = !DILocation(line: 6, column: 7, scope: !6)
!17 = distinct !{!17, !9, !18, !19}
!18 = !DILocation(line: 7, column: 3, scope: !6)
!19 = !{!"llvm.loop.isvectorized", i32 1}
!20 = distinct !{!20, !21}
!21 = !{!"llvm.loop.unroll.disable"}
!22 = !DILocation(line: 0, scope: !6)
!23 = !DILocation(line: 8, column: 3, scope: !6)
!24 = distinct !{!24, !9, !18, !25, !19}
!25 = !{!"llvm.loop.unroll.runtime.disable"}
!26 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 12, type: !7, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!27 = !DILocation(line: 14, column: 3, scope: !26)
!28 = !DILocation(line: 16, column: 10, scope: !26)
!29 = !DILocation(line: 16, column: 5, scope: !26)
!30 = !DILocation(line: 6, column: 10, scope: !6, inlinedAt: !31)
!31 = distinct !DILocation(line: 18, column: 11, scope: !26)
!32 = !DILocation(line: 6, column: 7, scope: !6, inlinedAt: !31)
!33 = !DILocation(line: 5, column: 3, scope: !6, inlinedAt: !31)
!34 = !DILocation(line: 19, column: 3, scope: !26)
!35 = !DILocation(line: 21, column: 1, scope: !26)
!36 = !DILocation(line: 20, column: 3, scope: !26)
